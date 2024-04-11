import dash
import dash_core_components as dcc
import dash_html_components as html
from dash.dependencies import Input, Output
import plotly.graph_objects as go
from walls import walls
from extended_functions import *

app = dash.Dash(__name__)

# Sample data for the grid
wall_pattern = walls()
state_reg = state_register()
state_reg.add("regenerate",0)
state_reg.add("maze",[])

app.layout = html.Div(children=[
    dcc.Graph(
        id='grid-graph'
    ),
    html.Button('Regenerate', id='regenerate_button',n_clicks=0)
    
])

@app.callback(
    Output('grid-graph', 'figure'),
    [Input('grid-graph', 'figure'),
     Input("grid-graph", "clickData"),
     Input("regenerate_button", "n_clicks")]# Empty dependency for initial render
)
def update_graph(fig,clickData,regenerate):
    state_reg.update_curr("regenerate",regenerate)
    state_reg.show()
     
    # Wall Generation
    fig = go.Figure()
    if state_reg.curr_state["regenerate"] != state_reg.prev_state["regenerate"]:
        for wall in wall_pattern.prune_walls():
            fig.add_trace(wall)
    
    # Updating the looks of the Maze
    fig.update_layout(
        height = 750,
        width = 750,
        margin=dict(l=0, r=0, t=0, b=0),
        paper_bgcolor="darkgrey",
        plot_bgcolor = "black",
        showlegend=False
        )
        
    fig.update_xaxes(visible=False,showgrid=False)
    fig.update_yaxes(visible=False,showgrid=False)
         
    state_reg.update_prev("regenerate",regenerate)   
    
    return fig
if __name__ == '__main__':
    app.run_server(debug=True)