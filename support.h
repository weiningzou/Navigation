//
//  supprt.h
//  project4
//
//  Created by 邹炜宁 on 3/13/17.
//  Copyright © 2017 邹炜宁. All rights reserved.
//

#ifndef support_h
#define support_h
#include "provided.h"
#include <algorithm>
using namespace std;
bool operator < (const GeoCoord& a,const GeoCoord& b);
bool operator == (const GeoCoord& a,const GeoCoord& b);
bool operator > (const GeoCoord& a,const GeoCoord& b);
bool operator != (const GeoCoord& a,const GeoCoord& b);
bool operator < (const GeoSegment& a,const GeoSegment& b);
bool operator == (const GeoSegment& a,const GeoSegment& b);
bool operator != (const GeoSegment& a,const GeoSegment& b);
bool operator > (const GeoSegment& a,const GeoSegment& b);
void printcoordinate(const StreetSegment& a);
void printNavSegment1(const NavSegment& a);
void printNavSegment2(const NavSegment& a);
string directionOfLine(const GeoSegment& gs);
class Compare
{
public:
    Compare( GeoCoord temp){

        e.latitudeText = temp.latitudeText;
        e.longitudeText = temp.longitudeText;
        e.latitude = temp.latitude;
        e.longitude = temp.longitude;
}

 bool operator() (GeoCoord a,GeoCoord b)
{
    if( distanceEarthKM(a,e) >  distanceEarthKM(b,e))
        return true;
        return false;
}
   
    GeoCoord e;
};
#endif /* supprt_h */
