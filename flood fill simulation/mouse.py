import plotly.graph_objects as go
import numpy as np
from matrix import matrix
import json

with open("config.json", 'r') as file:
    config = json.load(file)

class mouse(matrix):
    def __init__(self):
        super().__init__()
        self.visited_walls = []
        self.pos_x = 0
        self.pos_y = 0
        
    def plot(self):
    
        radius = 0.3
        theta = np.linspace(0, 2*np.pi, 100)
        x = self.pos_x + radius * np.cos(theta)
        y = self.pos_y + radius * np.sin(theta)
        
        return go.Scatter(x=x+0.5,
                          y=y+0.5,
                          mode='lines',
                          line_color = 'red')
        
    def move(self,direction): 
    
        if direction == "Up":   
            if self.pos_y + 1 <= config['GRID_SIZE_Y']-1:
                self.pos_y = self.pos_y + 1
                
        if direction == "Down":   
            if self.pos_y - 1 >= 0:
                self.pos_y = self.pos_y - 1
                
        if direction == "Right":   
            if self.pos_x + 1 <= config['GRID_SIZE_X']-1:
                self.pos_x = self.pos_x + 1
                
        if direction == "Left": 
            if self.pos_x - 1 >= 0:
                self.pos_x = self.pos_x - 1
        
    def sense(self,walls):
    
        '''
        self.sensor_data = {
                            "Up" : True if walls.find_wall_with_coordinates_in_wall_list(self.pos_x,self.pos_y+1,self.pos_x+1,self.pos_y+1) else False,
                            "Left" : True if walls.find_wall_with_coordinates_in_wall_list(self.pos_x,self.pos_y,self.pos_x,self.pos_y+1) else False,
                            "Down" : True if walls.find_wall_with_coordinates_in_wall_list(self.pos_x,self.pos_y,self.pos_x+1,self.pos_y) else False,
                            "Right" : True if walls.find_wall_with_coordinates_in_wall_list(self.pos_x+1,self.pos_y,self.pos_x+1,self.pos_y+1) else False
                        }
        '''
                        
        self.sensor_data = {
                            "Up" : walls.find_wall_with_coordinates_in_wall_list(self.pos_x,self.pos_y+1,self.pos_x+1,self.pos_y+1),
                            "Left" : walls.find_wall_with_coordinates_in_wall_list(self.pos_x,self.pos_y,self.pos_x,self.pos_y+1),
                            "Down" : walls.find_wall_with_coordinates_in_wall_list(self.pos_x,self.pos_y,self.pos_x+1,self.pos_y),
                            "Right" : walls.find_wall_with_coordinates_in_wall_list(self.pos_x+1,self.pos_y,self.pos_x+1,self.pos_y+1)
                        }
                        
        def check_validity(x,y):
            return (x,y) if x >= 0 and x < config['GRID_SIZE_X'] and y >= 0 and y < config['GRID_SIZE_Y'] else False
    
        self.neighbour_coodinates = {
                                    "Up" : check_validity(self.pos_x,self.pos_y+1) if self.sensor_data["Up"] else False,
                                    "Left" : check_validity(self.pos_x+1,self.pos_y) if self.sensor_data["Left"] else False,
                                    "Down" : check_validity(self.pos_x,self.pos_y-1) if self.sensor_data["Down"] else False,
                                    "Right" : check_validity(self.pos_x-1,self.pos_y) if self.sensor_data["Right"] else False
                                    }
        
        # Increases the opacity of the wall to let us know that the wall is detected 
        for direction,wall in self.sensor_data.items():
            if wall:
                wall.opacity = 1
                self.visited_walls.append(wall)
                
    def update(self):
        
        opposite_dir = {
                        "Up" : "Down",
                        "Down" : "Up",
                        "Left" : "Right",
                        "Right" : "Left"
                    }
        
        # Modify the lattice neigbours to None blocking the 
        lattice_point = self.find_lattice(self.pos_x,self.pos_y)
        for direction,wall in self.sensor_data.items():
            if wall:
                neighbour_lattice = lattice_point.children[direction]
                lattice_point.children[direction] = None
                if neighbour_lattice:
                    neighbour_lattice.children[opposite_dir[direction]] = None
                
        # Update the matrix
        self.flood_fill()
        
        if self.flood_fill_mode == "start_from_center":
        
            # Move the mouse to the neighbour with the min lattice value
            min_lattice_value = config["GRID_SIZE_X"] + config["GRID_SIZE_Y"]
            min_direction = None
            for direction,neighbour_lattice in lattice_point.children.items():
                if neighbour_lattice:
                    if neighbour_lattice.value < min_lattice_value:
                        min_lattice_value = neighbour_lattice.value
                        min_direction = direction
            
            self.move(min_direction)
        
        elif self.flood_fill_mode == "start_from_corner":
            
            # Move the mouse to the neighbour with the max lattice value
            max_lattice_value = 0     # Initialization
            max_direction = None
            for direction,neighbour_lattice in lattice_point.children.items():
                if neighbour_lattice:
                    if neighbour_lattice.value >  max_lattice_value:
                        max_lattice_value = neighbour_lattice.value
                        max_direction = direction
                        
            self.move(max_direction)
        
        print(max_direction)
        
    def reset_matrix(self):
        self.pos_x = 0
        self.pos_y = 0
        self.set_neighbours()    
        self.flood_fill()