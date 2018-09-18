#include "provided.h"
#include "MapLoader.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;


MapLoaderImpl::MapLoaderImpl()
{
    // Intentionally blank
}

MapLoaderImpl::~MapLoaderImpl()
{
    for (int i = 0; i < m_segments.size(); i++)
    {
        delete m_segments[i];  
    }
}

bool MapLoaderImpl::load(string mapFile)
{
    string file = mapFile;
    ifstream inStream;
    inStream.open(file);
    
    string line;
    GeoCoord beg;
    GeoCoord end;
    StreetSegment* newSeg;
    
    while (true)
    {
        if (getline(inStream,line))
        {
            newSeg = new StreetSegment;
            newSeg->streetName = line;
        }
        else
            break;
        if (getline(inStream,line))
        {
            bool result = stringToGeocoord(beg, end, line);
            if (!result)
                return false;
            
            newSeg->segment.start = beg;
            newSeg->segment.end = end;
        }
        else
            break;
        if (getline(inStream,line))
        {
            int dummy =  0;
            int numAttractions = stringtoNumber(dummy, line);
            
            if (numAttractions > 0)
                newSeg->attractions.resize(numAttractions);
            
            for (int i = 0; i < numAttractions; i++)
            {
                getline(inStream,line);
                
                string attractionGeoCoordString;
                int nextPos = 0, nextStop = 0;
                int substrLen = 0;
                GeoCoord attractionGeoCoord;
                string attractionName;
                
                nextStop = line.find('|', 0);
                substrLen = nextStop - nextPos;
                attractionName = line.substr(nextPos, substrLen);
                newSeg->attractions[i].name = attractionName;
                
                nextPos = nextStop + 1;
                attractionGeoCoordString = line.substr(nextPos, line.size() - nextPos);
                bool res = stringToGeocoord(attractionGeoCoord, attractionGeoCoordString);
                if (!res)
                    return false;
                newSeg->attractions[i].geocoordinates = attractionGeoCoord;
            }
        }
        else
            break;
        
        m_segments.push_back(newSeg);
    }
    return true;
}


bool MapLoaderImpl::stringToGeocoord(GeoCoord& theCoord, string theLine)
{
    if (theLine.size() == 0)
        return false;
    
    int substrLen = 0, nextPos = 0, nextStop = 0;
    
    string latOrLongString;
    double dummy = 0;
    
    ////////////// Set latitude fields of theCoord /////////////
    
    nextStop = theLine.find(',', nextPos);
    substrLen = nextStop - nextPos;
    latOrLongString = theLine.substr(nextPos, substrLen);
    
    theCoord.latitudeText =  latOrLongString;
    theCoord.latitude = stringtoNumber(dummy,latOrLongString);
    
    ////////////// Set longitude fields of theCoord /////////////
    
    nextPos = nextStop;

    
    while ((theLine[nextPos] == ',' || theLine[nextPos] == ' ') && nextPos < theLine.size()-1 )
        nextPos++;
    
    nextStop = theLine.find(' ', nextPos);
    if (nextStop == string::npos)
        nextStop = theLine.size();
    
    substrLen = nextStop - nextPos;

    
    latOrLongString = theLine.substr(nextPos, substrLen);

    theCoord.longitudeText =  latOrLongString;
    theCoord.longitude = stringtoNumber(dummy,latOrLongString);
    
    return true;
}

bool MapLoaderImpl::stringToGeocoord(GeoCoord& bCoord, GeoCoord& eCoord, string theLine)
{
    if (theLine.size() == 0)
        return false;
    
    string firstGeoString;
    string secondGeoString;
    int divider = theLine.find(' ', 0);
    
    if (theLine[divider - 1] == ',')
    {
        divider = theLine.find(' ', divider+1);
    }
    
    firstGeoString = theLine.substr(0, divider);
    secondGeoString = theLine.substr(divider+1, theLine.size() - divider - 1);
    
    bool res1 = stringToGeocoord(bCoord, firstGeoString);
    bool res2 = stringToGeocoord(eCoord, secondGeoString);
 
    if (res1 && res2)
        return true;
    else
        return false;
}

size_t MapLoaderImpl::getNumSegments() const
{
    return m_segments.size();
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if (segNum >= m_segments.size())
        return false;
    
    seg = *m_segments[segNum];
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
