#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include "AttractionMapper.h"
#include "SegmentMapper.h"
#include <string>
#include <vector>
using namespace std;

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
    bool canReachEnd(const MyMap<GeoCoord,GeoCoord>& map, const GeoCoord& gc, const string& attraction) const;
    StreetSegment findSegmentWithEndAndAtt(const GeoCoord& end, const GeoCoord& attraction) const;  
    StreetSegment findSegmentWithTwoEnds(const GeoCoord& g1, const GeoCoord& g2) const;
    vector<GeoCoord> findNextGeos(const GeoCoord& gc) const;
    string angleToDir(double ang) const;
    void addNavSegment(vector<NavSegment>& directions, const GeoCoord& curr, const GeoCoord& prev) const;
    vector<NavSegment> getNavSegments(const MyMap<GeoCoord,GeoCoord>& traveledRoutes,const GeoCoord& end) const;
    GeoSegment flipItsGeoSegment(StreetSegment seg, const GeoCoord& current, const GeoCoord& previous) const;
    void printAddedAssociation(const GeoCoord & curr, const GeoCoord & prev) const;
    string angleToTurnDir(double ang) const;
    MapLoader m_loader;
    AttractionMapper m_attract;
    SegmentMapper m_seg;
};

#endif
