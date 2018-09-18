#ifndef ATTRACTION_MAPPER_H
#define ATTRACTION_MAPPER_H
#include "MyMap.h"
#include "provided.h"
#include "MapLoader.h"
#include <string>
using namespace std;

class AttractionMapperImpl
{
public:
    AttractionMapperImpl();
    ~AttractionMapperImpl();
    void init(const MapLoader& ml);
    bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
    MyMap<string,GeoCoord> attractionMap;   
};

#endif
