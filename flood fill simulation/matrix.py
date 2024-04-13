import plotly.graph_objects as go
import numpy as np
import pandas as pd

GRID_SIZE = 16

class matrix:
    def __init__(self):
        self.reset()
        #self.matrix_value.to_csv("data.csv")
        
    def reset(self):
        grid_range = [i for i in range(GRID_SIZE)]
        self.x_coordinates,self.y_coordinates = np.meshgrid(grid_range, grid_range)
        
        matrix_value = np.array([[i+j for i in range(GRID_SIZE//2)] for j in range(GRID_SIZE//2)])
        matrix_value = np.vstack([np.flipud(matrix_value),matrix_value])
        matrix_value = np.hstack([np.fliplr(matrix_value),matrix_value])

        self.matrix_value = pd.DataFrame([self.x_coordinates.flatten(),self.y_coordinates.flatten(),matrix_value.flatten()])
        self.matrix_value = self.matrix_value.T
        self.matrix_value.columns = ['x','y','value']
        

    def get_value(self, x_coord, y_coord):

        # Filter the DataFrame for the matching x and y coordinates
        filtered_df = self.matrix_value[(self.matrix_value['x'] == x_coord) & (self.matrix_value['y'] == y_coord)]

        # Check if there's a match
        if len(filtered_df) > 0:
            return filtered_df["value"].values[0]  # Return the first value (assuming unique coordinates)
        else:
            raise ValueError("x_coordinate and y_coordinate columns not found in the DataFrame.")
          
    def update_value(self, x_coord, y_coord, value):

      # Try to find the row index using boolean indexing
      try:
        mask = (self.matrix_value['x'] == x_coord) & (self.matrix_value['y'] == y_coord)
        row_index = mask.idxmax()
      except ValueError:
        raise ValueError(f"Coordinates ({x_coord}, {y_coord}) not found in the DataFrame")

      # Set the value at the found index
      self.matrix_value.loc[row_index, 'value'] = value

     
    def graph(self):
        return go.Scatter(x=self.x_coordinates.flatten()+0.5,
                          y=self.y_coordinates.flatten()+0.5,
                          mode='text',
                          text=self.matrix_value['value'],
                          name='Scatter Plot')