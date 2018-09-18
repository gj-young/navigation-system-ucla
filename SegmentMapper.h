#ifndef SEGMENT_MAPPER_H
#define SEGMENT_MAPPER_H

#include "provided.h"
#include "MyMap.h"
#include <vector>
using namespace std;

class SegmentMapperImpl
{
public:
    SegmentMapperImpl();
    ~SegmentMapperImpl();
    void init(const MapLoader& ml);
    vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
    MyMap<GeoCoord,vector<StreetSegment>> segmentMap;
    bool lookUp(GeoCoord coord);
    void addToMap(GeoCoord coord, StreetSegment seg);
};

#endif
