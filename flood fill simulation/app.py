import dash
from dash import dcc,html
from dash.dependencies import Input, Output, State
import plotly.graph_objects as go
import os
from walls import walls
from extended_functions import memory
from mouse import mouse
import dash_mantine_components as dmc

app = dash.Dash(__name__, external_stylesheets=[])
server = app.server
wall_pattern = walls()
disk = memory()
moose = mouse()

app.layout = html.Div(children=[
            
    html.Div([
    
        html.H1("Maze Solving Simulator", style={'textAlign': 'center', 'marginBottom': '20px'}),
        html.P("Welcome to the Maze Solver Simulation App. Use the buttons and dropdowns below to interact with the maze.",
                style={'textAlign': 'center', 'marginBottom': '40px'}),
    
        html.Div([
            html.Div([
                html.H2("Help !!!", style={'textAlign': 'center', 'marginBottom': '20px', 'fontSize': '18px','width': '250px'}),
                html.P("Click on the Regenerate button to create a maze randomly. Edit the maze using add or delete wall button by clicking on it (it will turn red) and then clicking on adjacent nodes. Click on Clear Walls to remove all inner walls.",
                        style={'textAlign': 'center', 'marginBottom': '20px', 'fontSize': '16px','width': '250px'}),
                html.P("Click on the Step button multiple times to let the mouse move and simultaneously update the matrix. Click on the Reset button to restart the simulation.",
                        style={'textAlign': 'center', 'marginBottom': '20px', 'fontSize': '16px','width': '250px'}),
                html.P("Save mazes by giving them names and clicking the Save button. They can then be loaded or deleted by selecting them from the dropdown menus and clicking the respective buttons.",
                            style={'textAlign': 'center', 'marginBottom': '20px', 'fontSize': '16px','width': '250px'}),
            ], style={'textAlign': 'center', 'marginBottom': 'auto', 'marginRight': 'auto',
                      'display': 'flex', 'flex-direction': 'column', 'justifyContent': 'space-evenly',
                      'padding': '20px', 'align': '5','width': '250px'}),  # Added 'flex: 2' to make this division take up twice the space

            html.Div(
                dcc.Graph(
                    id='grid-graph',
                    style={'margin': 'auto'}  # Center align the graph
                ),
                style={'textAlign': 'center', 'marginBottom': '40px', 'marginRight': '80px'}
            ),

            html.Div([
                html.Div(id='selected_node_monitor'),
                dmc.Button('Delete Wall', id='delete_wall_button', color='black', style={'marginRight': '10px', 'align-self': 'center'}),
                dmc.Button('Add Wall', id='add_wall_button', color='black', style={'marginRight': '10px', 'align-self': 'center'}),
                dmc.Button('Regenerate', id='regenerate_button', color='black', style={'marginRight': '10px'}),
                dmc.Button('Clear walls', id='clear_wall_button', color='black', style={'marginRight': '10px'}),
                dmc.Button('Step', id='step_button', color='black', style={'marginRight': '10px'}),
                dmc.Button('Reset', id='reset_button', color='black', style={'marginRight': '10px'}),
                dmc.Alert(id='alert-delete-wall', hide=True, duration=1000),
            ], style={'textAlign': 'center', 'marginBottom': '40px', 'marginLeft': '40px',
                      'display': 'flex', 'flex-direction': 'column', 'justifyContent': 'space-evenly'}),

        ], style={'display': 'flex', 'flex-direction': 'row', 'justifyContent': 'space-evenly'}),

        html.Div([
            dcc.Input(id='save_filename', type='text', placeholder='Enter filename', style={'marginRight': '10px'}),
            dmc.Button('Save Maze', id='save_button', color='blue', variant='filled',
                       style={'marginRight': '10px'}),
        ], style={'textAlign': 'center', 'marginBottom': '40px'}),

        html.Div([
            dcc.Dropdown(id='load_dropdown', options=[], placeholder='Select file to load',
                         style={'marginRight': '10px'}),
            dmc.Button('Load Maze', id='load_button', color='blue', variant='filled',
                       style={'marginRight': '10px'}),
        ], style={'textAlign': 'center', 'marginBottom': '40px'}),

        html.Div([
            dcc.Dropdown(id='delete_dropdown', options=[], placeholder='Select file to delete',
                         style={'marginRight': '10px'}),
            dmc.Button('Delete Maze', id='delete_button', color='red', variant='filled',
                       style={'marginRight': '10px'}),
        ], style={'textAlign': 'center', 'marginBottom': '40px'}),
        
        # Footer
        html.Footer([
            html.P("Copyright © 2024"),
            html.P("Author: Sujit Justine Barwa"),
            html.P("Contact: Sujitjustine@gmail.com"),
            html.A("Source Code on GitHub", href="https://github.com/SujitJustineBarwa/Hackathon-2024", target="_blank"),
        ], style={'textAlign': 'center', 'marginTop': '40px'}),

    ], style={'maxWidth': '800px', 'margin': 'auto','display' : 'flex','flex-direction':'column'}),
])


@app.callback(
    [Output('grid-graph', 'figure'),
     Output('load_dropdown', 'options'),
     Output('delete_dropdown', 'options'),
     Output('selected_node_monitor', 'children'),
     Output('delete_wall_button', 'color'),
     Output('add_wall_button', 'color'),
     Output('alert-delete-wall', 'children'),
     Output('alert-delete-wall', 'hide')],
    [Input('grid-graph', 'clickData'),
     Input('regenerate_button', 'n_clicks'),
     Input('save_button', 'n_clicks'),
     Input('load_button', 'n_clicks'),
     Input('delete_button', 'n_clicks'),
     Input('delete_wall_button', 'n_clicks'),
     Input('reset_button', 'n_clicks'),
     Input('add_wall_button', 'n_clicks'),
     Input('clear_wall_button', 'n_clicks'),
     Input('step_button','n_clicks')],
    [State('grid-graph', 'figure'),
     State('save_filename', 'value'),
     State('load_dropdown', 'value'),
     State('delete_dropdown', 'value')]
)
def update_graph(clickData, regenerate_clicks, save_clicks, load_clicks, delete_clicks, delete_wall_clicks,
                 reset_clicks, add_wall_clicks,clear_wall_clicks,step_clicks, fig, save_filename, load_filename, delete_filename):
    ctx = dash.callback_context
    button_id = ctx.triggered[0]['prop_id'].split('.')[0]

    updated_fig = figure_template()  

    load_options = []
    delete_options = []

    filenames = [filename for filename in os.listdir() if filename.endswith('.txt')]
    load_options = [{'label': filename, 'value': filename} for filename in filenames]
    delete_options = [{'label': filename, 'value': filename} for filename in filenames]
    
    # Setting the disk memory and delete wall button color
    disk.add("delete_wall_enable", False)
    disk.add("add_wall_enable", False)
    disk.add("selected_nodes", [])
    disk.add("clicked_nodes", [])
    disk.add("delete_wall_button_color", '')
    disk.add("add_wall_button_color", '')
    disk.add("alert_message", '')
    disk.add("hide", False)
    
    disk.status["hide"] = True

    if button_id == 'regenerate_button':
        # Wall Generation
        for wall in wall_pattern.prune_walls():
            updated_fig.add_trace(wall.element())
            
    elif button_id == 'clear_wall_button':
        wall_pattern.wall_list = []
        wall_pattern.create_outer_walls()
        
    elif button_id == 'save_button':
        # Save maze to a file
        if fig and save_filename:
            filename = save_filename + '.txt'
            wall_pattern.to_text(filename)

    elif button_id == 'load_button':
        # Load maze from file
        if load_clicks > 0:
            for wall in wall_pattern.from_text(load_filename):
                updated_fig.add_trace(wall.element())

    elif button_id == 'delete_button':
        # Delete maze file
        if delete_clicks > 0:
            if os.path.exists(delete_filename):
                os.remove(delete_filename)      

    elif button_id == 'add_wall_button':
        if not disk.status["add_wall_enable"]:
            disk.status["add_wall_enable"] = True
            disk.status["add_wall_button_color"] = 'red'
            
            disk.status["delete_wall_enable"] = False
            disk.status["delete_wall_button_color"] = ''
                                                    
        else:
            disk.status["add_wall_enable"] = False
            disk.status["add_wall_button_color"] = ''
            
        # Resetting the disk memory and delete wall button color
        disk.status["selected_nodes"] = []
        disk.status["clicked_nodes"] = []
            
    elif button_id == 'delete_wall_button':
        if not disk.status["delete_wall_enable"]:
            disk.status["delete_wall_enable"] = True
            disk.status["delete_wall_button_color"] = 'red'
            
            disk.status["add_wall_enable"] = False
            disk.status["add_wall_button_color"] = ''
            
        else:
            disk.status["delete_wall_enable"] = False
            disk.status["delete_wall_button_color"] = ''

        # Resetting the disk memory and delete wall button color
        disk.status["selected_nodes"] = []
        disk.status["clicked_nodes"] = []
            
    elif clickData and disk.status["delete_wall_enable"]:
        node_x, node_y = clickData['points'][0]['x'], clickData['points'][0]['y']
        node = wall_pattern.find_Node(node_x, node_y)
        disk.status["selected_nodes"].append(f"Selected Node ID: {node.node_Id}")
        disk.status["clicked_nodes"].append(node)

        if len(disk.status["clicked_nodes"]) >= 2:
            return_status = wall_pattern.delete_wall(*disk.status["clicked_nodes"])
            if return_status:
                disk.status["alert_message"] = "Wall deleted !!!"
            else:
                disk.status["alert_message"] = "Wall not deleted !!!"
            disk.status["hide"] = False

            # Resetting the disk memory and delete wall button color
            disk.status["selected_nodes"] = []
            disk.status["clicked_nodes"] = []
            
    elif clickData and disk.status["add_wall_enable"]:
        node_x, node_y = clickData['points'][0]['x'], clickData['points'][0]['y']
        node = wall_pattern.find_Node(node_x, node_y)
        disk.status["selected_nodes"].append(f"Selected Node ID: {node.node_Id}")
        disk.status["clicked_nodes"].append(node)

        if len(disk.status["clicked_nodes"]) >= 2:
            return_status = wall_pattern.add_wall(*disk.status["clicked_nodes"])
            if return_status:
                disk.status["alert_message"] = "Wall added !!!"
            else:
                disk.status["alert_message"] = "Wall not added !!!"
            disk.status["hide"] = False

            # Resetting the disk memory and delete wall button color
            disk.status["selected_nodes"] = []
            disk.status["clicked_nodes"] = []
            
    elif button_id == 'step_button':
        moose.update()
        
    elif button_id == 'reset_button':
        # Reset matrix
        moose.reset_matrix()
        wall_pattern.reset_opacity()

    # Display the mouse
    moose.sense(wall_pattern)
    updated_fig.add_trace(moose.plot())
    
    # Displaying the matrix
    updated_fig.add_trace(moose.graph_matrix())
    
    # Replotting the walls
    if wall_pattern.wall_list:
        for wall in wall_pattern.wall_list:
            updated_fig.add_trace(wall.element())
            
    # Replotting all the nodes
    if wall_pattern.all_internal_nodes:
        for node in wall_pattern.all_internal_nodes + wall_pattern.all_edge_nodes:
            updated_fig.add_trace(node.element())

    return updated_fig, load_options, delete_options, disk.status["selected_nodes"], disk.status["delete_wall_button_color"],disk.status["add_wall_button_color"], disk.status["alert_message"], disk.status["hide"]


def figure_template():

    updated_fig = go.Figure()
        
    updated_fig.update_layout(
        height=750,
        width=750,
        margin=dict(l=0, r=0, t=0, b=0),
        paper_bgcolor="darkgrey",
        plot_bgcolor="black",
        showlegend=False
    )

    updated_fig.update_xaxes(visible=False, showgrid=False)
    updated_fig.update_yaxes(visible=False, showgrid=False)
    return updated_fig


if __name__ == '__main__':
    app.run_server(debug=True)
