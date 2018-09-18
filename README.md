# navigation-system-ucla
Local navigation system centered around UCLA, implemented using breadth-first search.

I found this project idea from the CS32 UCLA webpage and used the provided starting files: provided.h, validlocs.txt, and mapdata.txt. 
- provided.h - contains class definitions for the navigation system as well as basic formulas for geospatial calculations. 
- validlocs.txt - text file that contains a current list of starting and ending locations that the navigation system can handle.
- mapdata.txt - text file that contains thousands of geolocations around UCLA.

I implemented most of the classes for the project according to the suggested interface in provided.h and wrote a breadth-first search algorithm using the stl queue. I also implemented a map class (similar to the stl map class) used to organize the geolocation data for efficient search. 

Next steps:
- Improving the search speed 
- Additional geolocations 
- Graphical interface
  
Thank you to the UCLA professors for this project!
