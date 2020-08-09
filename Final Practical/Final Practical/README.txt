Name: Marc Molnar
Project: Final Practical

Additions:

Added A* algorithm to a maze of cubes on the floor.
All path values and cube positions are random based on a seed
Added in a creeper that will move along the path until it comes in contact with steve which stands on the final node
Path is lit up in comparrison to all the other cubes that make up the floor.
Creeper checks collision with the floor to see where it is within the path. Each time it is then chosen to move to the next path position.
Added the connections and path values that are held within MyEntity. 


Issues:

A longer loadup time as the A* algorithm is all done in the startup instead of in the update part of the program.

Controls:
No added controls other than the movement of the camera.