import plotly.graph_objects as go
import numpy as np
import pandas as pd
import json

with open("config.json", 'r') as file:
    config = json.load(file)

class lattice:
  def __init__(self,x,y):
    self.x = x
    self.y = y
    self.value = 0
    self.children = {
                      "Up" : None,
                      "Down" : None,
                      "Right" : None,
                      "Left" : None
                    }
    
class matrix:
  def __init__(self):
     
    self.all_lattice_points = []
    
    # Generate all the lattice points
    for x in range(config["GRID_SIZE_X"]):
      for y in range(config["GRID_SIZE_Y"]):
        temp_lattice_point = lattice(x,y)
        self.all_lattice_points.append(temp_lattice_point)
        
        if x == config["GRID_SIZE_X"]//2 and y == config["GRID_SIZE_Y"]//2:
          self.root = temp_lattice_point
          
    self.set_neighbours()    
    self.flood_fill_mode = "start_from_corner"
    self.flood_fill()
    
  def find_lattice(self,x,y):
    for lattice in self.all_lattice_points:
      if lattice.x == x and lattice.y == y:
        return lattice
      
  def set_neighbours(self):
    
    for lattice in self.all_lattice_points:
      temp_up_neighbour = self.find_lattice(lattice.x,lattice.y+1)
      temp_down_neighbour = self.find_lattice(lattice.x,lattice.y-1)
      temp_right_neighbour = self.find_lattice(lattice.x+1,lattice.y)
      temp_left_neighbour = self.find_lattice(lattice.x-1,lattice.y)
      if temp_up_neighbour:
        lattice.children["Up"] = temp_up_neighbour
      if temp_down_neighbour:
        lattice.children["Down"] = temp_down_neighbour
      if temp_left_neighbour:
        lattice.children["Left"] = temp_left_neighbour
      if temp_right_neighbour:
        lattice.children["Right"] = temp_right_neighbour
      
  # BFS algorithm
  def flood_fill(self):
  
    if config["GRID_SIZE_X"]%2 == 1 and config["GRID_SIZE_Y"]%2 == 1 and self.flood_fill_mode == "start_from_center":     # For odd size grid
        
      q = [self.root]
      level = [0]
      visited = [self.root]
      while q:
          current_lattice = q[0]
          current_lattice.value = level[0]
          del level[0]
          del q[0]
          
          for child in current_lattice.children.values():
            if child:
              if child not in visited:
                  level.append(current_lattice.value+1)
                  q.append(child)
                  visited.append(child)
                  
    elif config["GRID_SIZE_X"]%2 == 0 and config["GRID_SIZE_Y"]%2 == 0 and self.flood_fill_mode == "start_from_center":     # For even size grid
    
      q = [
           self.root,
           self.find_lattice(self.root.x-1,self.root.y),
           self.find_lattice(self.root.x-1,self.root.y-1),
           self.find_lattice(self.root.x,self.root.y-1)
          ]
           
      level = [0,0,0,0]
      visited = q.copy()
      while q:
          current_lattice = q[0]
          current_lattice.value = level[0]
          del level[0]
          del q[0]
          
          for child in current_lattice.children.values():
            if child:
              if child not in visited:
                  level.append(current_lattice.value+1)
                  q.append(child)
                  visited.append(child)
                  
    elif self.flood_fill_mode == "start_from_corner":
        
      q = [
            self.find_lattice(0,0),
            self.find_lattice(config["GRID_SIZE_X"]-1,0),
            self.find_lattice(config["GRID_SIZE_X"]-1,config["GRID_SIZE_Y"]-1),
            self.find_lattice(0,config["GRID_SIZE_Y"]-1),
          ]
      level = [0,0,0,0]
      visited = q.copy()
      while q:        
        current_lattice = q[0]
        current_lattice.value = level[0]
        del level[0]
        del q[0]
        
        for child in current_lattice.children.values():
          if child:
            if child not in visited:
                level.append(current_lattice.value+1)
                q.append(child)
                visited.append(child)
                  
      # Use this to check the lattice points value and there neighbour
      '''       
      for lattice in self.all_lattice_points:
          print(f"Point : {lattice.x,lattice.y} Value : {lattice.value}")
          for dir,child in lattice.children.items():
              if child:
                  print(f"Neighbour in {dir} : {child.x,child.y}")
          print()
      '''
    
  def graph_matrix(self):
  
    x_coordinates = []
    y_coordinates = []
    value = []
    
    for lattice_point in self.all_lattice_points:
      x_coordinates.append(lattice_point.x + 0.5)
      y_coordinates.append(lattice_point.y + 0.5)
      value.append(lattice_point.value)
      
    return go.Scatter(x=x_coordinates,
                      y=y_coordinates,
                      mode='text',
                      text=value
                    )

'''
class matrix:
    def __init__(self):
      self.reset_matrix()
      #self.matrix.to_csv("data.csv")
        
    def reset_matrix(self):
      
      grid_range_x = [i for i in range(config["GRID_SIZE_X"])]
      grid_range_y = [i for i in range(config["GRID_SIZE_Y"])]
      self.x_coordinates,self.y_coordinates = np.meshgrid(grid_range_x, grid_range_y)
        
      if config["GRID_SIZE_X"]%2 == 0 and config["GRID_SIZE_X"] == config["GRID_SIZE_Y"] :  # If GRID_SIZE is even and square
        
        matrix = np.array([[i+j for i in range(config["GRID_SIZE_X"]//2)] for j in range(config["GRID_SIZE_X"]//2)])
        matrix = np.vstack([np.flipud(matrix),matrix])
        matrix = np.hstack([np.fliplr(matrix),matrix])

        self.matrix = pd.DataFrame([self.x_coordinates.flatten(),self.y_coordinates.flatten(),matrix.flatten()])
        self.matrix = self.matrix.T
        self.matrix.columns = ['x','y','value']

      elif config["GRID_SIZE_X"]%2 != 0 and config["GRID_SIZE_X"] == config["GRID_SIZE_Y"]:  # If GRID_SIZE is odd and square
      
        matrix = np.array([[i+j for i in range(config["GRID_SIZE_X"]//2+1)] for j in range(config["GRID_SIZE_Y"]//2+1)])
        matrix = np.hstack([np.fliplr(matrix[:,1:]),matrix])
        matrix = np.vstack([np.flipud(matrix[1:,:]),matrix])

        self.matrix = pd.DataFrame([self.x_coordinates.flatten(),self.y_coordinates.flatten(),matrix.flatten()])
        self.matrix = self.matrix.T
        self.matrix.columns = ['x','y','value']
        

    def get_value(self, x_coord, y_coord):

      # Filter the DataFrame for the matching x and y coordinates
      filtered_df = self.matrix[(self.matrix['x'] == x_coord) & (self.matrix['y'] == y_coord)]

      # Check if there's a match
      if len(filtered_df) > 0:
          return filtered_df["value"].values[0]  # Return the first value (assuming unique coordinates)
      else:
          raise ValueError("x_coordinate and y_coordinate columns not found in the DataFrame.")
        
    def update_value(self, x_coord, y_coord, value):

      # Try to find the row index using boolean indexing
      try:
        mask = (self.matrix['x'] == x_coord) & (self.matrix['y'] == y_coord)
        row_index = mask.idxmax()
      except ValueError:
        raise ValueError(f"Coordinates ({x_coord}, {y_coord}) not found in the DataFrame")

      # Set the value at the found index
      self.matrix.loc[row_index, 'value'] = value

     
    def graph_matrix(self):
      return go.Scatter(x=self.x_coordinates.flatten()+0.5,
                        y=self.y_coordinates.flatten()+0.5,
                        mode='text',
                        text=self.matrix['value'],
                        name='Scatter Plot')
'''