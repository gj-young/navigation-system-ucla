#ifndef MAPLOADER_H
#define MAPLOADER_H

#include <string>
#include <sstream>
using namespace std;


template <typename T>
T stringtoNumber(T dummy, string theLine)  // added dummy variable so at least one parameter is of type T
{
    stringstream temp(theLine);
    T val;
    temp >> val;
    return val;
}

class MapLoaderImpl
{
public:
    MapLoaderImpl();
    ~MapLoaderImpl();
    bool load(string mapFile);
    size_t getNumSegments() const;
    bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
    vector<StreetSegment*> m_segments;
    bool stringToGeocoord(GeoCoord& theCoord, string theLine);
    bool stringToGeocoord(GeoCoord& bCoord, GeoCoord& eCoord, string theLine);
};

#endif
