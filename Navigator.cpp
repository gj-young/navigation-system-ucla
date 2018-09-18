#include "Navigator.h"
#include "provided.h"
#include "MapLoader.h"
#include "AttractionMapper.h"
#include "SegmentMapper.h"
#include <string>
#include <vector>
#include <iomanip>
#include <queue>
using namespace std;

NavigatorImpl::NavigatorImpl()
{
    // Intentionally blank
}

NavigatorImpl::~NavigatorImpl()
{
    // Intentionally blank
}

bool NavigatorImpl::loadMapData(string mapFile)
{
    bool res = m_loader.load(mapFile);
    if (!res)
        return false;
    m_attract.init(m_loader);
    m_seg.init(m_loader);
    
    
    
    
    return true;
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
    if (!directions.empty())
    {
        directions.clear();
    }
    
    GeoCoord startGc;
    GeoCoord endGc;
    bool validSrc = m_attract.getGeoCoord(start,startGc);
    bool validDest = m_attract.getGeoCoord(end,endGc);
    
    for (int i = 0; i < start.size(); i++)    // Temp solution because most items of the attraction vectors of street segments stored as Xxx Yyy
    {
        if (i == 0)
            start[i] = toupper(start[i]);
        if (i > 0 && start[i-1] == ' ')
            start[i] = toupper(start[i]);
    }
    
    for (int i = 0; i < end.size(); i++)    // Temp solution because most items of the attraction vectors of street segments stored as Xxx Yyy
    {
        if (i == 0)
            end[i] = toupper(end[i]);
        if (i > 0 && end[i-1] == ' ')
            end[i] = toupper(end[i]);
    }
    
    if (!validSrc)
    {
        return NAV_BAD_SOURCE;
    }
    if (!validDest)
    {
        return NAV_BAD_DESTINATION;
    }
    
    bool foundRoute = false;
    MyMap<GeoCoord,GeoCoord> pastRoutes;
    queue<GeoCoord> geoQ;
    vector<GeoCoord> nextGeos;
    vector<pair<GeoCoord,int>> discovered;   // GeoCoord and its round number
    GeoCoord empty;
    GeoCoord c;
    GeoCoord prev;
    
    int round = 0;
    int myRound = 0;
    geoQ.push(startGc);
    pair<GeoCoord, int> myPair(startGc,round);
    discovered.push_back(myPair);
    pastRoutes.associate(startGc,empty);
    
    while (!geoQ.empty())
    {
        prev = c;
        c = geoQ.front();
        geoQ.pop();
        int k;
        
        if (prev.latitudeText != "0")
        {
            vector <GeoCoord> possibleParents;

            for (int k = discovered.size()-1; k >= 0; k--)
            {
                if (discovered[k].first == c)
                    myRound = discovered[k].second;
            }

            for (int k = discovered.size()-1; k >= 0; k--)
            {
                
                if (discovered[k].second == (myRound - 1))
                    possibleParents.push_back(discovered[k].first);
            }
            
            StreetSegment ss;
            
            bool foundParent = false;
            
            for (k = 0; k < possibleParents.size(); k++)
            {
                if (possibleParents.size() == 1 && possibleParents[k] == startGc)
                {
                    foundParent = true;
                    prev = possibleParents[k];
                    break;
                }
                
                ss = findSegmentWithTwoEnds(c, possibleParents[k]);
                if (ss.streetName != "" && !(c==possibleParents[k]))
                {
                    foundParent = true;
                    prev = possibleParents[k];
                    break;
                }
            }
            pastRoutes.associate(c,prev);
        }
        
        if (canReachEnd(pastRoutes,c,end))
        {
            pastRoutes.associate(endGc,c);
            myPair.first = endGc;
            myPair.second = myRound+1;
            discovered.push_back(myPair);
            foundRoute = true;
            break;
        }

        nextGeos = findNextGeos(c);
        
        for (int i = 0; i < nextGeos.size(); i++)
        {
            bool alreadyPassed = false;
            for (int j = 0; j < discovered.size(); j++)
            {
                if (nextGeos[i] == discovered[j].first)
                {
                    alreadyPassed = true;
                    break;
                }
            }
            
            if (!alreadyPassed)
            {
                myPair.first = nextGeos[i];
                myPair.second = myRound+1;
                discovered.push_back(myPair);
                geoQ.push(nextGeos[i]);
            }
        }
    }
    
    if (foundRoute)
    {
        directions = getNavSegments(pastRoutes,endGc);
    }
    
    if (!foundRoute)
    {
        return NAV_NO_ROUTE;
    }
    else
        return NAV_SUCCESS;
}

void NavigatorImpl::printAddedAssociation(const GeoCoord & curr, const GeoCoord & prev) const   // added for debugging
{
    cout << setprecision(6) << fixed << curr.latitudeText << " " << curr.longitudeText
    << "<--" << prev.latitudeText << " " << prev.longitudeText << endl;
}

vector<NavSegment> NavigatorImpl::getNavSegments(const MyMap<GeoCoord,GeoCoord>& traveledRoutes,const GeoCoord& end) const
{
    vector<pair<GeoCoord,GeoCoord>> optimalRouteVect;
    GeoCoord c = end;
    const GeoCoord* prevPtr = traveledRoutes.find(c);
    vector<NavSegment> dirVector;
    while (prevPtr != nullptr)
    {
        pair<GeoCoord,GeoCoord> currPair (c,*prevPtr);
        optimalRouteVect.push_back(currPair);
        c = *prevPtr;
        prevPtr = traveledRoutes.find(c);
    }
    
    for (int i = optimalRouteVect.size() - 1; i >= 0; i--)
    {
        addNavSegment(dirVector,optimalRouteVect[i].first,optimalRouteVect[i].second);
    }
    
    return dirVector;
}

void NavigatorImpl::addNavSegment(vector<NavSegment>& directions, const GeoCoord& curr, const GeoCoord& prev) const
{
    if (prev.latitudeText == "0" && prev.longitudeText == "0")
        return;
    
    StreetSegment ss = findSegmentWithTwoEnds(curr,prev);
    
    if (ss.streetName == "")
    {
        ss = findSegmentWithEndAndAtt(curr, prev);
    }
    
    ss.segment = flipItsGeoSegment(ss, curr,prev);
    
    if (directions.size() > 0)
    {
        int prev = directions.size() - 1;
        string prevSt = directions[prev].m_streetName;
        GeoSegment prevGeo = directions[prev].m_geoSegment;
        if (ss.streetName != prevSt)
        {
            NavSegment turn;
            turn.m_command = NavSegment::TURN;
            turn.m_streetName = ss.streetName;
            turn.m_direction = angleToTurnDir(angleBetween2Lines(prevGeo,ss.segment));
            directions.push_back(turn);
        }
    }
    
    NavSegment ns;
    ns.m_streetName = ss.streetName;
    ns.m_geoSegment = ss.segment;   // Check if geoSegment is correct (might want effective geoSegment (shorter one))
    ns.m_direction = angleToDir(angleOfLine(ns.m_geoSegment));
    ns.m_command = NavSegment::PROCEED;
    ns.m_distance = distanceEarthMiles(curr, prev);
    
    directions.push_back(ns);
}

string NavigatorImpl::angleToTurnDir(double ang) const
{
    if (ang < 180.0)
        return "left";
    else
        return "right";
}

GeoSegment NavigatorImpl::flipItsGeoSegment(StreetSegment seg, const GeoCoord& current, const GeoCoord& previous) const
{
    GeoCoord g1 = seg.segment.start;
    GeoCoord g2 = seg.segment.end;
    if (g1 == previous && g2 == current)
    {
        return seg.segment;
    }
    else
    {
        GeoSegment gs;
        gs.start = previous;
        gs.end = current;
        return gs;
    }
}

string NavigatorImpl::angleToDir(double ang) const
{
    if (0 <= ang && ang <= 22.5)
        return "east";
    else if (22.5 < ang && ang <= 67.5)
        return "northeast";
    else if (67.5 < ang && ang <= 112.5)
        return "north";
    else if (112.5 < ang && ang <= 157.5)
        return "northwest";
    else if (157.5 < ang && ang <= 202.5)
        return "west";
    else if (202.5 < ang && ang <= 247.5)
        return "southwest";
    else if (247.5 < ang && ang <= 292.5)
        return "south";
    else if (292.5 < ang && ang <= 337.5)
        return "southeast";
    else if (337.5 < ang && ang < 360)
        return "east";
    else
        return "none";
}

vector<GeoCoord> NavigatorImpl::findNextGeos(const GeoCoord& gc) const
{
    const vector<StreetSegment> vect = m_seg.getSegments(gc);
    
    vector<GeoCoord> result;
    for (int i = 0; i < vect.size(); i++)
    {
        if (!(vect[i].segment.start == gc) && !(vect[i].segment.end == gc))
        {
            result.push_back(vect[i].segment.start);
            result.push_back(vect[i].segment.end);
        }
        else if (vect[i].segment.start == gc)
        {
            result.push_back(vect[i].segment.end);
        }
        else if (vect[i].segment.end == gc)
        {
            result.push_back(vect[i].segment.start);
        }
    }

    return result;
}

bool NavigatorImpl::canReachEnd(const MyMap<GeoCoord,GeoCoord>& map, const GeoCoord& gc, const string& attraction) const
{
    vector<GeoCoord> nextGeos = findNextGeos(gc);
    
    vector<StreetSegment> associatedSegments;
    for (int j = 0; j < nextGeos.size(); j++)
    {
        StreetSegment ss = findSegmentWithTwoEnds(nextGeos[j],gc);
        associatedSegments.push_back(ss);
    }
    
    for (int j = 0; j < associatedSegments.size(); j++)
    {
        for (int k = 0; k < associatedSegments[j].attractions.size(); k++)
        {
            if (associatedSegments[j].attractions[k].name == attraction)
                return true;
        }
    }
    
    return false;
}

StreetSegment NavigatorImpl::findSegmentWithEndAndAtt(const GeoCoord& end, const GeoCoord& attraction) const   
{
    vector <StreetSegment> vect1 = m_seg.getSegments(end);
    vector <StreetSegment> vect2 = m_seg.getSegments(attraction);
    for (int i = 0; i < vect1.size(); i++)
        for (int j = 0; j < vect2.size(); j++)
        {
            if (vect1[i].segment.start == vect2[j].segment.start && vect1[i].segment.end == vect2[j].segment.end)
                return vect1[i];
        }
    return StreetSegment();
}


StreetSegment NavigatorImpl::findSegmentWithTwoEnds(const GeoCoord& g1, const GeoCoord& g2) const
{
    const vector<StreetSegment> assocWithG1 = m_seg.getSegments(g1);
    const vector<StreetSegment> assocWithG2 = m_seg.getSegments(g2);
    StreetSegment commonSeg;
    
    for (int i = 0; i < assocWithG1.size() ; i++)
    {
        for (int j = 0; j < assocWithG2.size(); j++)
        {
            if (assocWithG1[i].streetName == assocWithG2[j].streetName)
            {
                if (assocWithG1[i].segment.start == g1 && assocWithG1[i].segment.end == g2)
                    commonSeg = assocWithG1[i];
                else if (assocWithG1[i].segment.end == g1 && assocWithG1[i].segment.start == g2)
                    commonSeg = assocWithG1[i];
            }
        }
    }
    return commonSeg;
}
   
//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}
