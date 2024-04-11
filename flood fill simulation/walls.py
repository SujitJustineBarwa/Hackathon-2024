import plotly.graph_objects as go
from collections import defaultdict
import random

GRID_SIZE = 16
 
class Node:
    def __init__(self, x=0, y=0):
        self.node_Id = f"Node_{x,y}"     # Unique id for nodes
        self.x = x
        self.y = y
        self.children = []
        
    def edge_Node(self):
        return self.x == 0 or self.y == 0 or self.x == GRID_SIZE or self.y == GRID_SIZE
   
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
        for i in range(GRID_SIZE+1):
            for j in range(GRID_SIZE+1):
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
    def __init__(self,node1,node2,line_color = 'green'):
        self.wall_id = node1.node_Id + '_' + node2.node_Id
        self.nodes = [node1,node2]
        self.x = (node1.x,node2.x)
        self.y = (node1.y,node2.y)
        self.line_color = line_color
        self.children = []
        
    def element(self):
        return go.Scatter(
                            x = self.x,
                            y = self.y,
                            line_color = self.line_color,
                         )
    
    def edge_Wall(self):
        return self.x == (0,0) or self.x == (GRID_SIZE,GRID_SIZE) or self.y == (0,0) or self.y == (GRID_SIZE,GRID_SIZE)
   
# Create walls
# Functionality :
#   1) Creates all the possible walls 
#   2) Prune some walls randomly
class walls(Node_map):
    def __init__(self):
        super().__init__()
        self.all_walls = defaultdict(list)
        self.generate_all_walls()
        
    def find_wall(self,node1,node2):
        for node,walls in self.all_walls.items():
            for wall in walls:
                if node1 in wall.nodes and node2 in wall.nodes:
                    return wall
        
    def generate_all_walls(self):
        for node in self.all_internal_nodes:
            if len(node.children) == 4:
                for child in node.children:
                    temp_wall = self.find_wall(node,child)
                    if temp_wall:
                        self.all_walls[node].append(temp_wall)
                    else:
                        self.all_walls[node].append(wall_element(node,child,line_color = 'blue'))
                        
        for node in self.all_edge_nodes:
            if len(node.children):
                for child in node.children:
                    temp_wall = self.find_wall(node,child)
                    if temp_wall:
                        self.all_walls[node].append(temp_wall)
                    else:
                        self.all_walls[node].append(wall_element(node,child,line_color = 'white'))
    
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
            
        
    def prune_walls(self,load_prune_walls = None):
        
        if not load_prune_walls:
            
            # Build the goal
            exceptions =  [self.find_Node(GRID_SIZE//2,GRID_SIZE//2),
                           self.find_Node(GRID_SIZE//2+1,GRID_SIZE//2),
                           self.find_Node(GRID_SIZE//2+1,GRID_SIZE//2+1),
                           self.find_Node(GRID_SIZE//2,GRID_SIZE//2+1),
                           self.find_Node(GRID_SIZE//2-1,GRID_SIZE//2+1),
                           self.find_Node(GRID_SIZE//2-1,GRID_SIZE//2),
                           self.find_Node(GRID_SIZE//2-1,GRID_SIZE//2-1),
                           self.find_Node(GRID_SIZE//2,GRID_SIZE//2-1)]
            
            self.wall_list = [self.all_walls[exceptions[1]][3],
                             self.all_walls[exceptions[2]][3],
                             self.all_walls[exceptions[3]][2],
                             self.all_walls[exceptions[4]][3],
                             self.all_walls[exceptions[5]][3],
                             self.all_walls[exceptions[6]][0],
                             self.all_walls[exceptions[7]][0],
                             #self.all_walls[exceptions[2]][2]
                             ]
            
            goal_walls =  self.wall_list.copy()
            
            for wall in self.wall_list:
                wall.line_color = "green"
            
            # Building the internal walls
            wall_built = [None]
            select_wall = None
            for node in self.all_internal_nodes:
                if node not in exceptions:
                    while select_wall in wall_built:
                        select_wall = random.choice(self.all_walls[node])
                    self.wall_list.append(select_wall)
                    wall_built.append(select_wall)
               
            # Remove the cyclic components
            self.set_children() # Resets the children
            for wall in self.wall_list:
                wall_to_remove = self.cyclic_check(wall)
                if wall_to_remove:
                    if wall_to_remove not in goal_walls:
                        self.wall_list.remove(wall_to_remove)
                        self.set_children()
                    
            # Building the edge walls
            wall_built = []
            for node in self.all_edge_nodes:
                for wall in self.all_walls[node]:
                    if wall not in wall_built:
                        self.wall_list.append(wall)
                
            return list(map(lambda x : x.element(),self.wall_list))
        
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
            
      
#a = Node_map()

# a = walls()
# wall_list = a.prune_walls()