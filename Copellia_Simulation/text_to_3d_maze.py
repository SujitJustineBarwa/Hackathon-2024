# Run in Blender scripts

import bpy
import re

file_path = r"C:\Users\BAW3KOR\Desktop\My_Research\Hackathon_2024\Hackathon-2024\sample.txt"

# Read the file
with open(file_path, 'r') as file:
    lines = file.readlines()

# Extract coordinates of walls
walls = []
for line in lines:
    line = line.strip()
    if line:
        coordinates = re.findall(r'\((\d+), (\d+)\)', line)
        for idx, coord in enumerate(coordinates):
            if idx == 0:
                start_coord = list(map(int, coord))
            else:
                end_coord = list(map(int, coord))
        
        walls.append([(start_coord[0], start_coord[1]), (end_coord[0], end_coord[1])])

# Setting the metrics
bpy.context.scene.unit_settings.system = 'METRIC'
bpy.context.scene.unit_settings.scale_length = 0.01
bpy.context.scene.unit_settings.length_unit = 'CENTIMETERS'

# Generate the walls
for wall in walls:
    start_coord = wall[0]
    end_coord = wall[1]
    height = 1

    # Determine wall orientation and thickness
    if start_coord[0] == end_coord[0]:  # Vertical wall
        thickness = 0.2
    else:  # Horizontal wall
        thickness = 0.2  

    # Define the vertices of the wall based on orientation
    if start_coord[0] == end_coord[0]:  # Vertical wall
        vertices = [
            (start_coord[0], start_coord[1], 0),
            (end_coord[0], end_coord[1], 0),
            (end_coord[0], end_coord[1], height),
            (start_coord[0], start_coord[1], height),
            (start_coord[0] - thickness / 2, start_coord[1], 0),
            (end_coord[0] - thickness / 2, end_coord[1], 0),
            (end_coord[0] - thickness / 2, end_coord[1], height),
            (start_coord[0] - thickness / 2, start_coord[1], height)
        ]
    else:  # Horizontal wall
        vertices = [
            (start_coord[0], start_coord[1], 0),
            (end_coord[0], end_coord[1], 0),
            (end_coord[0], end_coord[1], height),
            (start_coord[0], start_coord[1], height),
            (start_coord[0], start_coord[1] - thickness / 2, 0),
            (end_coord[0], end_coord[1] - thickness / 2, 0),
            (end_coord[0], end_coord[1] - thickness / 2, height),
            (start_coord[0], start_coord[1] - thickness / 2, height)
        ]
        
        
    # Define the faces of the wall
    faces = [
        (0, 1, 2, 3),        # Face 0
        (4, 5, 6, 7),        # Face 1
        (0, 4, 7, 3),        # Face 2
        (1, 5, 6, 2),        # Face 3
        (0, 1, 5, 4),        # Face 4
        (3, 2, 6, 7)         # Face 5
        ]


    # Create the mesh
    mesh = bpy.data.meshes.new(name=f"WallMesh_{start_coord}_{end_coord}")
    mesh.from_pydata(vertices, [], faces)

    # Create the object and link it to the scene
    obj = bpy.data.objects.new(name=f"Wall_{start_coord}_{end_coord}", object_data=mesh)
    bpy.context.scene.collection.objects.link(obj)