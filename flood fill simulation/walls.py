import plotly.graph_objects as go
from collections import defaultdict
import json
import random
 
with open("config.json", 'r') as file:
    config = json.load(file)
 
class Node:
    def __init__(self, x=0, y=0):
        self.node_Id = f"Node_{x,y}"     # Unique id for nodes
        self.x = x
        self.y = y
        self.children = []
        
    def edge_Node(self):
        return self.x == 0 or self.y == 0 or self.x == config['GRID_SIZE_X'] or self.y == config['GRID_SIZE_Y']
        
    def element(self):
        #return go.Scatter(x=[self.x], y=[self.y], marker_symbol='circle-open-dot', marker_size=12, marker_color='green')
        return go.Scatter(x=[self.x], y=[self.y], marker_symbol='circle-open-dot', marker_size=12)
   
# Create an instance with node points recursively connecting all the neigbour
# Functionality :
#   1) Creates a node if it was not created already
#   2) Finds a node given its coordinate
#   3) Maintains a list of internal and edge nodes
#   4) Edge nodes are only connected to Edge nodes but internal nodes are connected to internal as wells as edge nodes     
class Node_map:
    def __init__(self):
        self.all_internal_nodes = []
        self.all_edge_nodes = []
        self.generate_Nodes()
        self.set_neighbours()
        
    def generate_Nodes(self):
        for i in range(config['GRID_SIZE_X']+1):
            for j in range(config['GRID_SIZE_Y']+1):
                if not self.find_Node(i,j):
                    node = Node(i,j)
                    if not node.edge_Node():
                        self.all_internal_nodes.append(node)
                    else:
                        self.all_edge_nodes.append(node)
                    
    def find_Node(self,x,y):
        for node in self.all_internal_nodes:
            if node.x == x and node.y == y:
                return node
                
        for node in self.all_edge_nodes:
            if node.x == x and node.y == y:
                return node
                
    def set_neighbours(self):
        for node in self.all_internal_nodes:
            node.children = [
                            self.find_Node(node.x+1,node.y),
                            self.find_Node(node.x,node.y+1),
                            self.find_Node(node.x-1,node.y),
                            self.find_Node(node.x,node.y-1),
                            ]
        
        for node in self.all_edge_nodes:
            node.children = [
                            self.find_Node(node.x+1,node.y),
                            self.find_Node(node.x,node.y+1),
                            self.find_Node(node.x-1,node.y),
                            self.find_Node(node.x,node.y-1),
                            ]
            
            node.children = [x for x in node.children if x != None]
            node.children = [x for x in node.children if x.edge_Node()]   #Filters only the edge nodes
              
        
# Create wall element
# Functionality :
#   1) Creates a wall in plotly go
class wall_element:
    def __init__(self,node1,node2,line_color = 'green',opacity = 0.3):
        self.wall_id = node1.node_Id + '_' + node2.node_Id
        self.nodes = [node1,node2]
        self.x = (node1.x,node2.x)
        self.y = (node1.y,node2.y)
        self.line_color = line_color
        self.opacity = opacity
        self.children = []
        
    def element(self):
        return go.Scatter(
                            x = self.x,
                            y = self.y,
                            line_color = self.line_color,
                            opacity=self.opacity
                         )
        
    def edge_Wall(self):
        return self.x == (0,0) or self.x == (config['GRID_SIZE_X'],config['GRID_SIZE_X']) or self.y == (0,0) or self.y == (config['GRID_SIZE_Y'],config['GRID_SIZE_Y'])
   
# Create walls
# Functionality :
#   1) Generates all the possible walls 
#   2) Prune some walls randomly
class walls(Node_map):
    def __init__(self):
        super().__init__()
        self.all_walls = defaultdict(list)
        self.generate_all_walls()    # Generate wall but walls won't be visible
        self.wall_list = []
       
    def reset_opacity(self):
        for node,walls in self.all_walls.items():
            for wall in walls:
                wall.opacity = 0.3
        
    def find_wall_with_nodes(self,node1,node2):
        for node,walls in self.all_walls.items():
            for wall in walls:
                if node1 in wall.nodes and node2 in wall.nodes:
                    return wall
                    
    def find_wall_with_coordinates_in_wall_list(self,x1,y1,x2,y2):
        node1 = self.find_Node(x1,y1)
        node2 = self.find_Node(x2,y2)
        for node,walls in self.all_walls.items():
            for wall in walls:
                if node1 in wall.nodes and node2 in wall.nodes:
                    if wall in self.wall_list:
                        return wall
                    
    def find_wall_with_id(self,id):
        for node,walls in self.all_walls.items():
            for wall in walls:
                if wall.wall_id == id:
                    return wall
                    
    def generate_all_walls(self):
                            
        for node in self.all_internal_nodes:
            if len(node.children) == 4:
                for child in node.children:
                    temp_wall = self.find_wall_with_nodes(node,child)
                    if temp_wall:
                        self.all_walls[node].append(temp_wall)
                    else:
                        self.all_walls[node].append(wall_element(node,child,line_color = 'blue'))
                        
        for node in self.all_edge_nodes:
            if len(node.children):
                for child in node.children:
                    temp_wall = self.find_wall_with_nodes(node,child)
                    if temp_wall:
                        self.all_walls[node].append(temp_wall)
                    else:
                        self.all_walls[node].append(wall_element(node,child,line_color = 'white'))
                        
        if config['GRID_SIZE_X']%2 == 0 and config['GRID_SIZE_Y']%2 == 0:
            # Build the goal
            self.exceptions =  [self.find_Node(config['GRID_SIZE_X']//2,config['GRID_SIZE_Y']//2),
                                self.find_Node(config['GRID_SIZE_X']//2+1,config['GRID_SIZE_Y']//2),
                                self.find_Node(config['GRID_SIZE_X']//2+1,config['GRID_SIZE_Y']//2+1),
                                self.find_Node(config['GRID_SIZE_X']//2,config['GRID_SIZE_Y']//2+1),
                                self.find_Node(config['GRID_SIZE_X']//2-1,config['GRID_SIZE_Y']//2+1),
                                self.find_Node(config['GRID_SIZE_X']//2-1,config['GRID_SIZE_Y']//2),
                                self.find_Node(config['GRID_SIZE_X']//2-1,config['GRID_SIZE_Y']//2-1),
                                self.find_Node(config['GRID_SIZE_X']//2,config['GRID_SIZE_Y']//2-1)]
                                
            self.goal_walls = [self.all_walls[self.exceptions[1]][3],
                               self.all_walls[self.exceptions[2]][3],
                               self.all_walls[self.exceptions[3]][2],
                               self.all_walls[self.exceptions[4]][3],
                               self.all_walls[self.exceptions[5]][3],
                               self.all_walls[self.exceptions[6]][0],
                               self.all_walls[self.exceptions[7]][0],
                               self.all_walls[self.exceptions[2]][2]]
                            
            for wall in self.goal_walls:
                wall.line_color = "green"
                
        else:
        
            # Build the goal
            self.exceptions =  [self.find_Node(config['GRID_SIZE_X']//2,config['GRID_SIZE_Y']//2),
                                self.find_Node(config['GRID_SIZE_X']//2+1,config['GRID_SIZE_Y']//2),
                                self.find_Node(config['GRID_SIZE_X']//2+1,config['GRID_SIZE_Y']//2+1),
                                self.find_Node(config['GRID_SIZE_X']//2,config['GRID_SIZE_Y']//2+1)]
                                
            self.goal_walls = [self.all_walls[self.exceptions[1]][1],
                          self.all_walls[self.exceptions[2]][2],
                          self.all_walls[self.exceptions[3]][3],
                          self.all_walls[self.exceptions[0]][0]]
                            
            for wall in self.goal_walls:
                wall.line_color = "green"
            
    
    def set_children(self):
        for wall in self.wall_list:
            node1,node2 = wall.nodes
            wall.children = []
            for wall_1 in self.all_walls[node1]:
                if wall_1 in self.wall_list and wall != wall_1:
                    wall.children.append(wall_1)
                    
            for wall_2 in self.all_walls[node2]:
                if wall_2 in self.wall_list and wall != wall_2:
                    wall.children.append(wall_2)
            
        
    def create_goal_walls(self):
        
        if config['GRID_SIZE_X']%2 == 0 and config['GRID_SIZE_Y']%2 == 0:
            self.wall_list =   self.goal_walls[:-1]   
        else:
            self.wall_list =   self.goal_walls[:-1]
                            
           
    def create_outer_walls(self): 
        # Building the edge walls
        wall_built = []
        for node in self.all_edge_nodes:
            for wall in self.all_walls[node]:
                if wall not in wall_built:
                    self.wall_list.append(wall)
        
    def prune_walls(self):
        
        with open("config.json", 'r') as file:
            global config
            config = json.load(file)
    
        self.wall_list = []
        self.create_goal_walls()     # Creates wall and added them in wall list that is visible
        self.create_outer_walls()    # Creates wall and added them in wall list that is visible
        
        # Building the internal walls
        wall_built = [None]
        select_wall = None
        for node in self.all_internal_nodes:
            if node not in self.exceptions:
                while select_wall in wall_built:
                    select_wall = random.choice(self.all_walls[node])
                self.wall_list.append(select_wall)
                wall_built.append(select_wall)

                        
        return self.wall_list
        
    def cyclic_check(self,wall):
        
        q = [wall]
        visited = [wall]
        while q:

            wall = q.pop()
            for child in wall.children:
                if child not in q and child not in visited:
                    q.append(child)
                    visited.append(child)
                elif child in q:
                    return child     # This child is causing cyclic nature
                    
        return False
        
    def to_text(self,name):
        with open(name, 'w+') as file:
            # Registering the all wall ID
            for wall in self.wall_list:
                file.write(wall.wall_id + "\n")
            file.close()
            
    def from_text(self,name):
        self.wall_list = []
        #self.create_goal_walls()
        #self.create_outer_walls()
                        
        with open(name, 'r') as file:
            # Registering the all wall ID
            wall_id_list = file.readlines()
            for wall_id in wall_id_list:
                self.wall_list.append(self.find_wall_with_id(wall_id[:-1]))    # -1 removes the \n char that was added in to_text()
            file.close()
            
        return self.wall_list
        
    def delete_wall(self,node1,node2):
        wall = self.find_wall_with_nodes(node1,node2)
        if wall:
            self.wall_list.remove(wall)
            return True   # Return status
        else:
            return False    # Return status
            
    def add_wall(self,node1,node2):
        wall = self.find_wall_with_nodes(node1,node2)
        if wall:
            if wall not in self.wall_list:
                self.wall_list.append(wall)
                return True   # Return status
        else:
            return False    # Return status
      
#a = Node_map()

# a = walls()
# wall_list = a.prune_walls()