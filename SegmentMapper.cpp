#include "provided.h"
#include <vector>
#include "MyMap.h"
#include "support.h"
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
    MyMap<GeoCoord,vector<StreetSegment>>* m_map;
    
};

SegmentMapperImpl::SegmentMapperImpl()
{
    m_map = new MyMap<GeoCoord, vector<StreetSegment>>;
}

SegmentMapperImpl::~SegmentMapperImpl()
{
    delete m_map;
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
    
    size_t size = ml.getNumSegments();
    int i = 0;
    StreetSegment seg;
    while(i < size+1)
    {
        vector<StreetSegment> ss, gg, ee;
        //cerr<<"run"<<endl;
        ml.getSegment(i, seg);

        if(m_map->find(seg.segment.start) == nullptr)
        {
            //cerr<<seg.segment.start.latitudeText<<" "<<seg.segment.start.longitudeText<<endl;
           ss.push_back(seg);
            m_map->associate(seg.segment.start, ss);
        }
        else
        {
            m_map->find(seg.segment.start) -> push_back(seg);
        }
        //v = m_map.find(seg.segment.end);
        if(m_map->find(seg.segment.end) == nullptr)
        {
            //cerr<<"dosomething";
            ee.push_back(seg);
            m_map->associate(seg.segment.end, ee);
        }
        else
        {
            m_map->find(seg.segment.end) -> push_back(seg);
        }
        if(!(seg.attractions.empty()))
        {
            for(auto p = seg.attractions.begin(); p != seg.attractions.end();p++)
            {
//                string g1 = p -> geocoordinates.latitudeText;
//                string g2 = p -> geocoordinates.longitudeText;
//                string g = g1 + g2;
                //v = m_map.find(p -> geocoordinates);
                if(m_map->find(p -> geocoordinates) == nullptr)
                {
                    gg.push_back(seg);
                    m_map->associate(p -> geocoordinates, gg);
                }
                else
                {
                    m_map->find(p -> geocoordinates) -> push_back(seg);
                }

            }
        }
       
        i++;
    }

}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
//    string target1 = gc.latitudeText;
//    string target2 = gc.longitudeText;
//    string target = target1 + target2;
     //cerr<<"run"<<endl;
	vector<StreetSegment> segments;
    if(m_map->find(gc) == nullptr)
    {//cerr<<"nullptr"<<endl;
        return segments;
    }
    segments = *(m_map->find(gc));
    for(auto p = segments.begin(); p != segments.end();p++)
    {
        //cerr<<p->streetName<<endl;
        //printcoordinate(*p);
    }
    //cerr<<gc.latitudeText<<" "<<gc.longitudeText;
   
	return segments;
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
