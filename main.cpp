// Credit to UCLA professor Carey Nachenberg
// Main routine to call the navigation system and output its directions

#include "provided.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
using namespace std;

string directionOfLine(const GeoSegment& gs)
{
    double ang = angleOfLine(gs);
    
    if (0 <= ang && ang <= 22.5)     // Check later - code duplication here 
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

void printDirectionsRaw(string start, string end, vector<NavSegment>& navSegments);
void printDirections(string start, string end, vector<NavSegment>& navSegments);

int main(int argc, char *argv[])
{

    bool raw = false;
	if (argc == 5  &&  strcmp(argv[4], "-raw") == 0)
	{
		raw = true;
		argc--;
	}
	if (argc != 4)
	{
		cout << "Usage: BruinNav mapdata.txt \"start attraction\" \"end attraction name\"" << endl
		   << "or" << endl
		   << "Usage: BruinNav mapdata.txt \"start attraction\" \"end attraction name\" -raw" << endl;
		return 1;
	}
	
	Navigator nav;
    string mapFile = argv[1];  // changed

	if ( ! nav.loadMapData(mapFile)) // changed
	{
        cout << "Map data file was not found or has bad format: " << argv[1] << endl;
		return 1;
	}

	if ( ! raw)
		cout << "Routing..." << flush;

	string start = argv[2];
	string end = argv[3];
	vector<NavSegment> navSegments;
    
	NavResult result = nav.navigate(start, end, navSegments);
	if ( ! raw)
		cout << endl;

	switch (result)
	{
	  case NAV_NO_ROUTE:
		cout << "No route found between " << start << " and " << end << endl;
		break;
	  case NAV_BAD_SOURCE:
		cout << "Start attraction not found: " << start << endl;
		break;
	  case NAV_BAD_DESTINATION:
		cout << "End attraction not found: " << end << endl;
		break;
	  case NAV_SUCCESS:
		if (raw)
			printDirectionsRaw(start, end, navSegments);
		else
			printDirections(start, end, navSegments);
		break;
	} 
}

void printDirectionsRaw(string start, string end, vector<NavSegment>& navSegments)
{
	cout << "Start: " << start << endl;
	cout << "End:   " << end << endl;
	cout.setf(ios::fixed);
	cout.precision(4);
	for (auto ns : navSegments)
	{
		switch (ns.m_command)
		{
		  case NavSegment::PROCEED:
			cout << ns.m_geoSegment.start.latitudeText << ","
			     << ns.m_geoSegment.start.longitudeText << " "
			     << ns.m_geoSegment.end.latitudeText << ","
			     << ns.m_geoSegment.end.longitudeText << " "
			     << ns.m_direction << " "
			     << ns.m_distance << " "
			     << ns.m_streetName << endl;
			break;
		  case NavSegment::TURN:
			cout << "turn " << ns.m_direction << " " << ns.m_streetName << endl;
			break;
		}
	}
}

void printDirections(string start, string end, vector<NavSegment>& navSegments)
{
	cout.setf(ios::fixed);
	cout.precision(2);

	cout << "You are starting at: " << start << endl;

	double totalDistance = 0;
	string thisStreet;
	GeoSegment effectiveSegment;
	double distSinceLastTurn = 0;

	for (auto ns : navSegments)
	{
		switch (ns.m_command)
		{
		  case NavSegment::PROCEED:
			if (thisStreet.empty())
			{
				thisStreet = ns.m_streetName;
				effectiveSegment.start = ns.m_geoSegment.start;
			}
			effectiveSegment.end = ns.m_geoSegment.end;
			distSinceLastTurn += ns.m_distance;
			totalDistance += ns.m_distance;
			break;
		  case NavSegment::TURN:
			if (distSinceLastTurn > 0)
			{
				cout << "Proceed " << distSinceLastTurn << " miles "
				     << directionOfLine(effectiveSegment) << " on " << thisStreet << endl;

				thisStreet.clear();
				distSinceLastTurn = 0;
			}
			cout << "Turn " << ns.m_direction << " onto " << ns.m_streetName << endl;
			break;
		}
	}

	if (distSinceLastTurn > 0)
    {
        cout << "Proceed " << distSinceLastTurn << " miles "
        << directionOfLine(effectiveSegment) << " on " << thisStreet << endl;
    }
	cout << "You have reached your destination: " << end << endl;
	cout.precision(1);
	cout << "Total travel distance: " << totalDistance << " miles" << endl;
}

