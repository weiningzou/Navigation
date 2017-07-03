//
//  support.cpp
//  project4
//
//  Created by 邹炜宁 on 3/13/17.
//  Copyright © 2017 邹炜宁. All rights reserved.
//
#include "support.h"
#include <stdio.h>
#include <iostream>
#include "MyMap.h"
using namespace std;
string directionOfLine(const GeoSegment& gs)
{
    double angle1;
    angle1 = angleOfLine(gs);
    if(angle1 >= 0 && angle1 <= 22.5)
        return "east";
    else if(angle1 > 22.5 && angle1 <= 67.5)
        return "northeast";
    else if(angle1 > 67.5 && angle1 <= 112.5)
        return "north";
    else if(angle1 > 112.5 && angle1 <= 157.5)
        return "northwest";
    else if(angle1 > 157.5 && angle1 <= 202.5)
        return "west";
    else if(angle1 > 202.5 && angle1 <= 247.5)
        return "southwest";
    else if(angle1 > 247.5 && angle1 <= 292.5)
        return "south";
    else if(angle1 > 292.5 && angle1 <= 337.5)
        return "southeast";
    else if(angle1 > 337.5 && angle1 <= 360)
        return "east";
    else
        return "error";

}

bool operator < (const GeoCoord& a,const GeoCoord& b)
{
    if(a.latitudeText < b.latitudeText)
        return true;
    else if(a.latitudeText == b.latitudeText)
        return a.longitudeText < b.longitudeText;
    return false;
}

bool operator == (const GeoCoord& a,const GeoCoord& b)
{
    if(a.longitudeText == b.longitudeText && a.latitudeText == b.latitudeText)
        return true;
    return false;
}

bool operator > (const GeoCoord& a,const GeoCoord& b)
{
    if(a.latitudeText > b.latitudeText)
        return true;
    else if(a.latitudeText == b.latitudeText)
        return a.longitudeText > b.longitudeText;

    return false;
}
bool operator != (const GeoCoord& a,const GeoCoord& b)
{
    if(a.longitudeText != b.longitudeText || a.latitudeText != b.latitudeText)
        return true;
    return false;
}

bool operator < (const GeoSegment& a,const GeoSegment& b)
{
    if(a.start < b.start)
        return true;
    else if(a.start == b.start)
        return a.end < b.end;
    return false;
}

bool operator == (const GeoSegment& a,const GeoSegment& b)
{
    if(a.start == b.start && a.end == b.end)
        return true;
    return false;
}

bool operator > (const GeoSegment& a,const GeoSegment& b)
{
    if(a.start > b.start)
        return true;
    else if(a.start == b.start)
        return a.end > b.end;
    return false;
}
bool operator != (const GeoSegment& a,const GeoSegment& b)
{
    if (a == b)
        return false;
    else
        return true;
}
void printcoordinate(const StreetSegment& a)
{
    cerr<<a.streetName<<endl;
    cerr<<a.segment.start.latitudeText<<","<<a.segment.start.longitudeText<<" "<<a.segment.end.latitudeText<<","<<a.segment.end.longitudeText<<endl;
    for(auto p = a.attractions.begin(); p!= a.attractions.end();p++)
    {
        cerr<<p->name<<"|"<<p->geocoordinates.latitudeText<<","<<p->geocoordinates.longitudeText<<endl;
    }
}

void printNavSegment1(const NavSegment& a)
{
    cerr<<"type: PROCEED"<<endl;
    cerr<<"start "<<a.m_geoSegment.start.latitudeText<<","<<a.m_geoSegment.start.longitudeText<<endl;
    cerr<<"end "<<a.m_geoSegment.end.latitudeText<<","<<a.m_geoSegment.end.longitudeText<<endl;
    cerr<<"direction  "<<a.m_direction<<endl;
    cerr<<"distance"<<a.m_distance<<endl;
    cerr<<"street "<<a.m_streetName<<endl<<endl<<endl;
}
void printNavSegment2(const NavSegment& a)
{
    cerr<<"type: TURN"<<endl;
    cerr<<"direction "<<a.m_direction<<endl;
    cerr<<"street "<<a.m_streetName<<endl<<endl<<endl;
}

