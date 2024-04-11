# Simulation for flood fill algorithm

In this section, a python code is presented where a web app has been designed which simulates the maze and the working of the flood fill algorithm.The dash library is used.

**<u>How to use this section of the code</u>**
1) Create a new environment
2) Run pip install -r requirements.txt
3) Run app.py

**The Architect of the code**
![alt text](Architecture.PNG)

**Structure of walls.py**
![alt text](structure_of_wall.PNG)
1) Class Node:
    a) This class is for representing a point in space.
    b) A unique id is assigned to every node.
    
2) Class Node_map:
    a) This class generates the unique node points and set the neighbouring points(East,West,North,South).
    b) Two seperate nodes_lists are maintained 
        i) all_internal_nodes
        ii) all_edge_nodes
        Note : The edge nodes are note allow to keep the internal nodes as there children(neighour) while internal nodes can keep edge nodes as there children(neighour).This is done so that in furthur while in the development freedom could be given to the internal nodes
    c) A functionality is provide where you can find the node via its x and y coordinate.
    
3) Class wall_element:
    a) It stores two nodes and also what color it should be when plotted.

4) Class walls:
    a) It create node map and generate all the possible walls.
    b) The functionality then prune_walls() performs the following operation:
        i) Makes the goal walls.
        ii) Prune internal walls randomly.
        iii) Set the neighbour of the each wall and ensures that there are no closed loop forming(function cyclic_check() removes the cyclic walls using DFS algorithm). 
        iv) Makes the outer walls.
        v) Sends the walls to App.py for plotting
        
**Structure of App.py**
![alt text](structure_of_App.PNG)
    a) The app.layout is used to structure the placement of the graph object,text,buttons,etc
    b) The callback function is used update the graph object in the web app.
    
**Extended_Function.py**
    a) This function serves as a event manager function for the web app.
