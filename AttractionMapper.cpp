#include "AttractionMapper.h"
#include "provided.h"
#include "MyMap.h"
#include <string>
using namespace std;


AttractionMapperImpl::AttractionMapperImpl()
{
    // intentionally blank
}

AttractionMapperImpl::~AttractionMapperImpl()
{
    // intentionally blank
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
    StreetSegment mySeg;
    
    for (int i = 0; i < ml.getNumSegments(); i++)
    {
        if (ml.getSegment(i,mySeg))
        {
            for (int j = 0; j < mySeg.attractions.size(); j++)
            {
                string attName;
                GeoCoord attCoord;
                
                attName = mySeg.attractions[j].name;
                attCoord = mySeg.attractions[j].geocoordinates;
                
                for (int k = 0; k < attName.size(); k++)
                    attName[k] = tolower(attName[k]);
                
                attractionMap.associate(attName, attCoord);
            }
        }
    }
    
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
    for (int k = 0; k < attraction.size(); k++)
        attraction[k] = tolower(attraction[k]);
    
    const GeoCoord* attLoc = attractionMap.find(attraction);
    if (attLoc != nullptr)
    {
        gc = *attLoc;
        return true;
    }
    return false;
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions

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
