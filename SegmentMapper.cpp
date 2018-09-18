#include "SegmentMapper.h"
#include "provided.h"
#include "MyMap.h"
#include <vector>
using namespace std;

SegmentMapperImpl::SegmentMapperImpl()
{
    // intentionally blank
}

SegmentMapperImpl::~SegmentMapperImpl()
{
    // intentionally blank
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
    StreetSegment mySeg;
    GeoCoord begCoord;
    GeoCoord endCoord;
    GeoCoord attrCoord;
    
    for (int j = 0; j < ml.getNumSegments(); j++)
    {
        ml.getSegment(j, mySeg);
        begCoord = mySeg.segment.start;
        endCoord = mySeg.segment.end;
        
        addToMap(begCoord, mySeg);
        addToMap(endCoord, mySeg);
        
        for (int k = 0; k < mySeg.attractions.size(); k++)
            addToMap(mySeg.attractions[k].geocoordinates, mySeg);
    }
}

void SegmentMapperImpl::addToMap(GeoCoord coord, StreetSegment seg)
{
    if (!lookUp(coord))
    {
        vector<StreetSegment> myVect;
        myVect.push_back(seg);
        segmentMap.associate(coord,myVect);
    }
    else
    {
        vector<StreetSegment>* ptr = segmentMap.find(coord);
        if (ptr != nullptr)
        {
            ptr->push_back(seg);
        }
    }
}

bool SegmentMapperImpl::lookUp(GeoCoord coord)
{
    vector<StreetSegment> * segListPtr = segmentMap.find(coord);
    if (segListPtr != nullptr)
        return true;
    else
        return false;
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
    // Error! Cannot initalize variable of type vector<StreetSegment>* with r-value of type const vector<StreetSegment>*
    // Think it was calling the const version of find on segmentMap because the function is tagged const - won't modify data structures of object
    // So I stored the return value of find in a const pointer
    // Since I only need to return only a copy of the vector<StreetSegment> anyway
    
    vector<StreetSegment> emptyVec;
    const vector<StreetSegment>* segListPtr = segmentMap.find(gc);
    if (segListPtr != nullptr)
    {
        vector<StreetSegment> returnVec = *segListPtr;
        return returnVec;
    }
    else
    {
        return emptyVec;
    }
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
