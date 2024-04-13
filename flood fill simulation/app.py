import dash
import dash_core_components as dcc
import dash_html_components as html
from dash.dependencies import Input, Output, State
import plotly.graph_objects as go
from matrix import matrix
import os
from walls import walls
from extended_functions import *
import dash_mantine_components as dmc

app = dash.Dash(__name__, external_stylesheets=[])
wall_pattern = walls()
disk = memory()
mat = matrix()

app.layout = html.Div(children=[
    html.Div([
        html.H1("Maze Builder", style={'textAlign': 'center', 'marginBottom': '20px'}),
        html.P("Welcome to the Maze Builder app. Use the buttons and dropdowns below to interact with the maze.",
        html.P("Welcome to the Maze Builder app. Use the buttons and dropdowns below to interact with the maze.",
               style={'textAlign': 'center', 'marginBottom': '40px'}),
        html.Div(
            dcc.Graph(
                id='grid-graph',
                style={'margin': 'auto'}  # Center align the graph
            ),
            style={'textAlign': 'center', 'marginBottom': '40px'}
        ),
        html.Div([
            html.Div(id='selected_node_monitor'),
            dmc.Button('Delete Wall', id='delete_wall_button', color='black', style={'marginRight': '10px'}),
            dmc.Button('Regenerate', id='regenerate_button', color='black', variant='filled',
                       style={'marginBottom': '40px', 'marginLeft': '10px', 'marginRight': '10px'}),
            dmc.Button('Reset Matrix', id='reset_matrix_button', color='black', style={'marginRight': '10px'}),
            dmc.Alert(id='alert-delete-wall', hide=True,duration=1000),
        ], style={'textAlign': 'center', 'marginBottom': '40px'}),
        html.Div([
            dcc.Input(id='save_filename', type='text', placeholder='Enter filename', style={'marginRight': '10px'}),
            dmc.Button('Save Maze', id='save_button', color='blue', variant='filled',
                       style={'marginRight': '10px'}),
            dmc.Button('Save Maze', id='save_button', color='blue', variant='filled',
                       style={'marginRight': '10px'}),
        ], style={'textAlign': 'center', 'marginBottom': '40px'}),
        html.Div([
            dcc.Dropdown(id='load_dropdown', options=[], placeholder='Select file to load',
                         style={'marginRight': '10px'}),
            dmc.Button('Load Maze', id='load_button', color='blue', variant='filled',
                       style={'marginRight': '10px'}),
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
            dcc.Dropdown(id='delete_dropdown', options=[], placeholder='Select file to delete',
                         style={'marginRight': '10px'}),
            dmc.Button('Delete Maze', id='delete_button', color='red', variant='filled',
                       style={'marginRight': '10px'}),
        ], style={'textAlign': 'center', 'marginBottom': '40px'}),
    ], style={'maxWidth': '800px', 'margin': 'auto'}),
], style={'padding': '20px'})


@app.callback(
    [Output('grid-graph', 'figure'),
     Output('load_dropdown', 'options'),
     Output('delete_dropdown', 'options'),
     Output('selected_node_monitor', 'children'),
     Output('delete_wall_button', 'color'),
     Output('alert-delete-wall', 'children'),
     Output('alert-delete-wall', 'hide')],
     Output('delete_wall_button', 'color'),
     Output('alert-delete-wall', 'children'),
     Output('alert-delete-wall', 'hide')],
    [Input('grid-graph', 'clickData'),
     Input('regenerate_button', 'n_clicks'),
     Input('save_button', 'n_clicks'),
     Input('load_button', 'n_clicks'),
     Input('delete_button', 'n_clicks'),
     Input('delete_wall_button', 'n_clicks'),
     Input('reset_matrix_button', 'n_clicks')],
    [State('grid-graph', 'figure'),
     State('save_filename', 'value'),
     State('load_dropdown', 'value'),
     State('delete_dropdown', 'value')]
)
def update_graph(clickData, regenerate_clicks, save_clicks, load_clicks, delete_clicks, delete_wall_clicks,
                 reset_clicks, fig, save_filename, load_filename, delete_filename):
    ctx = dash.callback_context
    button_id = ctx.triggered[0]['prop_id'].split('.')[0]

    updated_fig = figure_template()  


    load_options = []
    delete_options = []

    filenames = [filename for filename in os.listdir() if filename.endswith('.txt')]
    load_options = [{'label': filename, 'value': filename} for filename in filenames]
    delete_options = [{'label': filename, 'value': filename} for filename in filenames]

    if button_id == 'regenerate_button':
        # Wall Generation
        for wall in wall_pattern.prune_walls():
            updated_fig.add_trace(wall.element())

    elif button_id == 'save_button':
        # Save maze to a file
        if fig and save_filename:
            filename = save_filename + '.txt'
            wall_pattern.to_text(filename)
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

    elif button_id == 'reset_matrix_button':
        # Reset matrix
        mat.reset()

    # Setting the disk memory and delete wall button color
    disk.add("delete_wall_enable", False)
    disk.add("selected_nodes", [])
    disk.add("clicked_nodes", [])
    disk.add("delete_wall_button_color", '')
    disk.add("alert_message", '')
    disk.add("hide", False)
    
    disk.status["hide"] = True

    if button_id == 'delete_wall_button':
        if not disk.status["delete_wall_enable"]:
            disk.status["delete_wall_enable"] = True
            disk.status["delete_wall_button_color"] = 'red'
        else:
            disk.status["delete_wall_enable"] = False
            disk.status["delete_wall_button_color"] = ''
            
            # Resetting the disk memory and delete wall button color
            disk.status["selected_nodes"] = []
            disk.status["clicked_nodes"] = []
            
    if clickData and disk.status["delete_wall_enable"]:
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

    # Displaying the matrix
    updated_fig.add_trace(mat.graph())
    
    # Replotting the walls
    if wall_pattern.wall_list:
        for wall in wall_pattern.wall_list:
            updated_fig.add_trace(wall.element())
            

    return updated_fig, load_options, delete_options, disk.status["selected_nodes"], disk.status["delete_wall_button_color"], disk.status["alert_message"], disk.status["hide"]


def figure_template():

    updated_fig = go.Figure()
        
    updated_fig.update_layout(
        height=750,
        width=850,
        margin=dict(l=0, r=0, t=0, b=0),
        paper_bgcolor="darkgrey",
        plot_bgcolor="black",
        showlegend=False
    )
        height=750,
        width=850,
        margin=dict(l=0, r=0, t=0, b=0),
        paper_bgcolor="darkgrey",
        plot_bgcolor="black",
        showlegend=False
    )

    updated_fig.update_xaxes(visible=False, showgrid=False)
    updated_fig.update_yaxes(visible=False, showgrid=False)
    return updated_fig

    return updated_fig


if __name__ == '__main__':
    app.run_server(debug=True)
