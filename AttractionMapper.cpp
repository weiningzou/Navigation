#include "provided.h"
#include <string>
#include "MyMap.h"
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
    MyMap<string,GeoCoord> m_map;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
    size_t size = ml.getNumSegments();
    int i = 0;
    StreetSegment seg;
    while(i < size)
    {
        ml.getSegment(i, seg);
        if(!(seg.attractions.empty()))
        {
            for(vector<Attraction>::iterator p = seg.attractions.begin(); p != seg.attractions.end();p++)
            {
                for(unsigned int j = 0; j < p -> name.length(); j++)
                {
                    p -> name[j] = tolower(p -> name[j]);
                }
                //cerr << p -> name << endl;
                m_map.associate(p -> name, p -> geocoordinates);
            }
        }
        i++;
    }
    
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
    
    for(unsigned int i = 0; i < attraction.length(); ++i)
        attraction[i] = tolower(attraction[i]);
    cerr<<attraction<<endl;
    if(m_map.find(attraction) == nullptr)
    {cerr<<"some error"<<endl;
        return false;}
    gc = *(m_map.find(attraction));
    cerr<<gc.latitudeText<<" "<<gc.longitudeText<<endl;
    return true;
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
