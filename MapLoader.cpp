#include "provided.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "support.h"
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
    //vector<StreetSegment*> getALlSegments();
private:
    vector<StreetSegment*> m_seg;
    size_t m_numsegments;
};

MapLoaderImpl::MapLoaderImpl()
{
    m_numsegments = -1;
}

MapLoaderImpl::~MapLoaderImpl()
{
}

bool MapLoaderImpl::load(string mapFile)
{
    StreetSegment* seg = nullptr;
    //int line = 0;
    ifstream infile(mapFile);
    if(! infile)
    {
        cerr<<"cannot load data"<<endl;
        return false;
    }
    string s;
    while(getline(infile, s))
    {
        seg = new StreetSegment;
        
        seg -> streetName = s;
        //cerr<<s<<endl;
        string waste;
        string geo[4];
        char delimeter(',');
        getline(infile,geo[0],delimeter);
        //cerr<<geo[0]<<endl;
        infile>>geo[1];
        //cerr<<geo[1]<<endl;
        getline(infile,waste,' ');
        getline(infile,geo[2],delimeter);
        //cerr<<geo[2]<<endl;
        infile>>geo[3];
        //cerr<<geo[3]<<endl;
        infile.ignore(1000,'\n');
        GeoCoord g1(geo[0],geo[1]);
        //cerr<<g1.latitudeText<<" "<<g1.longitudeText<<endl;
        GeoCoord g2(geo[2],geo[3]);
        //cerr<<g2.latitudeText<<" "<<g2.longitudeText<<endl;
        GeoSegment g (g1,g2);
        //printcoordinate(g);
        if(seg != nullptr)
            seg -> segment = g;
        int k;
        infile >> k;
        //cerr<<k<<endl;
        infile.ignore(1000,'\n');
        if(k == 0)
            m_seg.push_back(seg);
        else
        {
           while(k > 0)
           {
               Attraction a;
               getline(infile,s,'|');
               a.name = s;
               //cerr<<"Attraction"<<s<<endl;
               string s1, s2;
               getline(infile,s1,',');
               //cerr<<s1<<endl;
               infile >> s2;
               //cerr<<s2<<endl<<endl<<endl<<endl;
               infile.ignore(1000,'\n');
               GeoCoord geo1(s1,s2);
               a.geocoordinates = geo1;
               seg -> attractions.push_back(a);
               k--;
            }
            m_seg.push_back(seg);
        }
        //printcoordinate(*seg);
        m_numsegments++;
    }
    return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	return m_numsegments; // This compiles, but may not be correct
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
    if(segNum > m_numsegments)
        return false;
    seg = *(m_seg.at(segNum));
    //cerr<<seg.streetName<<endl;
    GeoSegment g;
    g = seg.segment;
    GeoCoord g1 = g.start;
    GeoCoord g2 = g.end;
    //cerr<<g1.latitudeText<<" "<<g1.longitudeText<<" "<<g2.latitudeText<<" "<<g2.longitudeText<<endl;
    return true;
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
