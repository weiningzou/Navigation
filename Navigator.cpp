#include "provided.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include "MyMap.h"
#include "support.h"
using namespace std;

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
    MapLoader* a;
    SegmentMapper* s;
    AttractionMapper* attraction;
    //vector<NavSegment> dir;
};

NavigatorImpl::NavigatorImpl()
{
    a = new MapLoader;
    s = new SegmentMapper;
    attraction = new AttractionMapper;
}

NavigatorImpl::~NavigatorImpl()
{
    delete a;
    delete s;
    delete attraction;
}

bool NavigatorImpl::loadMapData(string mapFile)
{
    if(a -> load(mapFile) == false)
    {
        cerr<<"somethingwrong"<<endl;
        return false;
    }
    s -> init(*a);
    attraction -> init(*a);
    return true;
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
    if(!directions.empty())
        directions.clear();
    vector<GeoCoord> pathway;
    vector<StreetSegment> places;
    MyMap<GeoCoord, GeoCoord> route;
    MyMap<GeoSegment, StreetSegment> visited;
    

    GeoCoord gstart , gend;
    double totaldistance = 0;
    if(!attraction -> getGeoCoord(start, gstart))
        return NAV_BAD_SOURCE;
    if(!attraction -> getGeoCoord(end, gend))
        return NAV_BAD_DESTINATION;
    Compare a(gend);
    priority_queue<GeoCoord,vector<GeoCoord>,Compare> geo(a);
    geo.push(gstart);
    while (!geo.empty()){
        GeoCoord current_point = geo.top();
        geo.pop();
        if (current_point == gend)
        {
            cerr<<"success"<<endl;
            if(gstart == gend)
            {
                GeoSegment g;
                g.start = gstart;
                g.end = gend;
               NavSegment na("stay", s -> getSegments(gstart).front().streetName, 0, g);
                directions.push_back(na);
                //printNavSegment1(na);
                return NAV_SUCCESS;
            }
            GeoCoord path1 = gend;
            //cerr<<path1.latitudeText<<" "<<path1.longitudeText<<endl;
            pathway.push_back(path1);
            GeoCoord path2;
            while(path2 != gstart)
            {
                path2 = *route.find(path1);
                pathway.push_back(path2);
                //cerr<<path2.latitudeText<<" "<<path2.longitudeText<<endl;
                path1 = path2;
            }
            cerr<<"get route!!!"<<endl;
 
            string firstName = s -> getSegments(gstart).front().streetName;
            for(int i = pathway.size()-1; i > 0; i--)
            {
                double angle1,angle2, distance; string direc1,direc2; GeoSegment g;
                g.start = pathway.at(i);
                g.end = pathway.at(i - 1);
                 distance = distanceEarthKM(g.start,g.end);
                angle1 = angleOfLine(g);
                if(angle1 >= 0 && angle1 <= 22.5)
                    direc1 = "east";
                else if(angle1 > 22.5 && angle1 <= 67.5)
                    direc1 = "northeast";
                else if(angle1 > 67.5 && angle1 <= 112.5)
                        direc1 = "north";
                else if(angle1 > 112.5 && angle1 <= 157.5)
                    direc1 = "northwest";
                else if(angle1 > 157.5 && angle1 <= 202.5)
                        direc1 = "west";
                else if(angle1 > 202.5 && angle1 <= 247.5)
                    direc1 = "southwest";
                else if(angle1 > 247.5 && angle1 <= 292.5)
                    direc1 = "south";
                else if(angle1 > 292.5 && angle1 <= 337.5)
                    direc1 = "southeast";
                else if(angle1 > 337.5 && angle1 <= 360)
                    direc1 = "east";
               

                bool action = false;
                vector<StreetSegment> temp = s -> getSegments(pathway.at(i-1));
                for(auto p = temp.begin(); p != temp.end(); p++)
                {
                    if (p -> streetName == firstName)
                    { NavSegment na( direc1, p -> streetName, distance, g);
                        directions.push_back(na);
                        //printNavSegment1(na);
                        totaldistance += na.m_distance;
                        action = true;
                        break;
                    }
                    
                }
                if(action == false)
                {
                    angle2 = angleBetween2Lines(directions.back().m_geoSegment, g);

                    if(angle2 < 180)
                        direc2 = "left";
                    else if(angle2 > 180)
                        direc2 = "right";

                    NavSegment na(direc2,temp.front().streetName);
                    directions.push_back(na);
                    //printNavSegment2(na);
                    NavSegment nb( direc1, temp.front().streetName, distance, g);
                    directions.push_back(nb);
                    //printNavSegment1(nb);
                    totaldistance += nb.m_distance;
                    firstName = temp.front().streetName;

                }
                
            }
            cerr<<endl<<endl;
            cerr<<"total distance is "<<totaldistance<<" kms"<<endl;
            return NAV_SUCCESS;}
        places = s -> getSegments(current_point);
        for(auto p = places.begin(); p != places.end(); p++)
        {
            if(visited.find(p -> segment) == nullptr)
               {
                   if(p -> segment.start == current_point)
                   {
                       geo.push(p -> segment.end);
                       route.associate(p -> segment.end, current_point);
                       //cerr<<"push end"<<endl;
                       visited.associate(p -> segment, *p);
                       for(auto f = p -> attractions.begin(); f != p -> attractions.end(); f++)
                       {
                           geo.push(f -> geocoordinates);
                           if(f -> geocoordinates != current_point)
                           route.associate(f -> geocoordinates, current_point);
                           //cerr<<"push attraction"<<endl;
                       }
                   }
                   else if(p -> segment.end == current_point)
                   {
                       geo.push(p -> segment.start);
                       route.associate(p -> segment.start, current_point);
                       //cerr<<"push start"<<endl;
                       visited.associate(p -> segment, *p);
                       for(auto f = p -> attractions.begin(); f != p -> attractions.end(); f++)
                       {
                           geo.push(f -> geocoordinates);
                           if(f -> geocoordinates != current_point)
                           route.associate(f -> geocoordinates, current_point);
                           //cerr<<"push attraction"<<endl;
                       }
                   }
                   else
                   {
                       for(auto f = p -> attractions.begin(); f != p -> attractions.end(); f++)
                       {
                           if(f -> geocoordinates == current_point)
                           {
                               geo.push(p -> segment.start);
                               route.associate(p -> segment.start, current_point);
                               geo.push(p -> segment.end);
                               route.associate(p -> segment.end, current_point);
                               //cerr<<"push both"<<endl;
                               for(auto q = p -> attractions.begin(); q != p -> attractions.end(); q++)
                               {
                                   if(q -> geocoordinates != current_point)
                                   {
                                       geo.push(q -> geocoordinates);
                                       route.associate(q -> geocoordinates, current_point);
                                       //cerr<<"push attraction"<<endl;
                                   }
                                   
                               }
                               visited.associate(p -> segment, *p);
                               break;
                           }
                       }
                   }

               }
        }

    }
    cerr<<"no route"<<endl;
    return NAV_NO_ROUTE;
}
//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}
