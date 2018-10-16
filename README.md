# navigation-system-ucla
Navigation system centered around UCLA, implemented using breadth-first search. The user specifies a starting and ending location from the list of 600+ locations (found in validlocs.txt) and the program outputs step-by-step directions based on the calculated optimal route. 

Idea from an old project on: 
- https://www.cs.ucla.edu/
- Accompanying files with geolocation data: provided.h, validlocs.txt, and mapdata.txt

Classes implemented: 
- MapLoader.h/.cpp
  - Processes a map data text file (containing all geolocations of interest) into a vector of street segments
- MyMap.h
  - A basic version of STL map implemented using a binary search tree
- AttractionMapper.h/.cpp
  - Processes the street segments vector of MapLoader to find the geocoordinate associated with each attraction and store the results in a     MyMap object 
- SegmentMapper.h/.cpp  
  - Processes the street segments vector of MapLoader to find all street segments associated with a given geocoordinate and stores the         result in a MyMap object 
- Navigator.h/.cpp
  - Holds member variables MapLoader, SegmentMapper, and AttractionMapper
  - Given a map data text file, initalizes its MapLoader and then uses it to initialize SegmentMapper and AttractionMapper
  - Given a starting and ending attraction, stores the calculated navigation directions in a vector
  
Next steps (to be continued):
- Improving the search speed 
- Additional geolocations 
- Graphical user interface
