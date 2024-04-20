# Simulation for flood fill algorithm

In this section, a python code is presented where a web app has been designed which simulates the maze and the working of the flood fill algorithm.The dash library is used.

## **<u>How to use this section of the code</u>**
1. Create a new environment
2. Run pip install -r requirements.txt
3. Run app.py

## **The Architect of the code**
![alt text](Architecture.PNG)

## **Structure of walls.py**
![alt text](structure_of_wall.PNG)
1. Class Node:<br>
    - This class is for representing a point in space.<br>
    - A unique id is assigned to every node.<br>
    
3. Class Node_map:<br>
   - This class generates the unique node points and set the neighbouring points(East,West,North,South).<br>
   - Two seperate nodes_lists are maintained <br>
        - all_internal_nodes <br>
        - all_edge_nodes <br>
        (Note : The edge nodes are note allow to keep the internal nodes as there children(neighour) while internal nodes can keep edge nodes as there children(neighour).This is done so that in furthur while in the development freedom could be given to the internal nodes.)<br>
   - A functionality is provide where you can find the node via its x and y coordinate.<br>
    
4. Class wall_element:<br>
   - It stores two nodes and also what color it should be when plotted.<br>

5. Class walls:<br>
   - It create node map and generate all the possible walls.<br>
   - The functionality then prune_walls() performs the following operation:<br>
      - Makes the goal walls.<br>
      - Prune internal walls randomly.<br>
      - Set the neighbour of the each wall and ensures that there are no closed loop forming(function cyclic_check() removes the cyclic walls using DFS algorithm).<br> 
      - Makes the outer walls.<br>
      - Sends the walls to App.py for plotting.<br>
        
## **Structure of App.py**<br>
![alt text](structure_of_App.PNG)<br>
   - The app.layout is used to structure the placement of the graph object,text,buttons,etc<br>
   - The callback function is used update the graph object in the web app.<br>
    
## **Extended_Function.py**<br>
   - This function serves as a event manager function for the web app.<br>

## **Mouse**<br> 
   - Path tracking is capability added.<br>
