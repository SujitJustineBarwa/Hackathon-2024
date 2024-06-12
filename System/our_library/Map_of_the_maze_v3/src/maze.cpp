#include "maze.h"
#include "Wall.h"
#include <limits>
#include <set>
#include <cmath>
#include <list>

Maze::Maze() {
    createGrid();
    setNeighbors();
}

Maze::~Maze() {
    cleanUp();
}

void Maze::createGrid() {
    grid.resize(GRID_ROWS, std::vector<cell*>(GRID_COLS));
    for (int i = 0; i < GRID_ROWS; ++i) {
        for (int j = 0; j < GRID_COLS; ++j) {
            grid[i][j] = new cell(i, j);
            grid[i][j]->setwalls();
        }
    }
}

void Maze::setNeighbors() {
    for (int i = 0; i < GRID_ROWS; ++i) {
        for (int j = 0; j < GRID_COLS; ++j) {
            cell* currentCell = grid[i][j];

            if (i > 0) {
                currentCell->addNeighborCell(grid[i - 1][j]);
            }

            if (i < GRID_ROWS - 1) {
                currentCell->addNeighborCell(grid[i + 1][j]);
            }

            if (j > 0) {
                currentCell->addNeighborCell(grid[i][j - 1]);
            }

            if (j < GRID_COLS - 1) {
                currentCell->addNeighborCell(grid[i][j + 1]);
            }
        }
    }
}

void Maze::display() const {
    for (int i = 0; i < GRID_ROWS; ++i) {
        for (int j = 0; j < GRID_COLS; ++j) {
            cell* currentCell = grid[i][j];
            std::cout << "Cell (" << currentCell->getX() << ", " << currentCell->getY() << ") value: " << currentCell->getValue() << " neighbors: ";
            for (cell* neighbor : currentCell->getNeighborCells()) {
                std::cout << "(" << neighbor->getX() << ", " << neighbor->getY() << ") ";
            }
            std::cout << std::endl;
        }
    }
}

void Maze::cleanUp() {
    for (int i = 0; i < GRID_ROWS; ++i) {
        for (int j = 0; j < GRID_COLS; ++j) {
            delete grid[i][j];
        }
    }
}

void Maze::floodFill(int startX, int startY) {

    std::queue<cell*> q;
    std::set<cell*> visited;

    if (GRID_ROWS%2 != 0 && GRID_ROWS%2 != 0)
    {
        q.push(grid[startX][startY]);
        visited.insert(grid[startX][startY]);
    }
    else
    {
        q.push(grid[startX][startY]);
        q.push(grid[startX][startY-1]);
        q.push(grid[startX-1][startY]);
        q.push(grid[startX-1][startY-1]);
        visited.insert(grid[startX][startY]);
        visited.insert(grid[startX][startY-1]);
        visited.insert(grid[startX-1][startY]);
        visited.insert(grid[startX-1][startY-1]);
    }

    while (!q.empty()) {
        cell* current = q.front();
        q.pop();

        for (cell* neighbor : current->getNeighborCells()) {
            if (visited.find(neighbor) == visited.end()) {
                neighbor->setValue(current->getValue() + 1);
                q.push(neighbor);
                visited.insert(neighbor);
            }
        }
    }
}


void Maze::find_wall_coordinates_and_update_walls(float x, float y, float theta, float front_TOF_reading, float back_TOF_reading,float left_TOF_reading,float right_TOF_reading) {
    //float theta_rad = theta * M_PI / 180.0;  // Convert theta to radians
    float theta_rad = theta;

    if (front_TOF_reading <= VALID_RANGE_TOF_READING){

        // Wall at the front of the Bot
        float wall_x = x + front_TOF_reading * std::cos(theta_rad);
        float wall_y = y + front_TOF_reading * std::sin(theta_rad);
        std::cout<<"The front wall coordinates : "<<wall_x<<" , "<<wall_y<<std::endl;

        this-> wall_update(wall_x,wall_y);
    }


    if (back_TOF_reading <= VALID_RANGE_TOF_READING){

        // Wall at the back of the Bot
        float wall_x = x - back_TOF_reading * std::cos(theta_rad);
        float wall_y = y - back_TOF_reading * std::sin(theta_rad);
        std::cout<<"The back wall coordinates : "<<wall_x<<" , "<<wall_y<<std::endl;

        this-> wall_update(wall_x,wall_y);
    }

    if (right_TOF_reading <= VALID_RANGE_TOF_READING){

        // Wall at the back of the Bot
        float wall_x = x - right_TOF_reading * std::sin(theta_rad);
        float wall_y = y + right_TOF_reading * std::cos(theta_rad);
        std::cout<<"The left wall coordinates : "<<wall_x<<" , "<<wall_y<<std::endl;

        this-> wall_update(wall_x,wall_y);
    }

    if (left_TOF_reading <= VALID_RANGE_TOF_READING){

        // Wall at the back of the Bot
        float wall_x = x + left_TOF_reading * std::sin(theta_rad);
        float wall_y = y - left_TOF_reading * std::cos(theta_rad);
        std::cout<<"The right wall coordinates : "<<wall_x<<" , "<<wall_y<<std::endl;

        this-> wall_update(wall_x,wall_y);
    }
}

void Maze::wall_update(float wall_x, float wall_y) {

    float x1, x2, y1, y2;
    int minDir = -1; // Direction associated with the minimum area
    int i_min,j_min;

    for (int i = 0; i < GRID_ROWS; ++i) {
        for (int j = 0; j < GRID_COLS; ++j) {

            x1 = grid[i][j]->north_wall_start_x;
            x2 = grid[i][j]->north_wall_end_x;
            y1 = grid[i][j]->north_wall_start_y;
            y2 = grid[i][j]->north_wall_end_y;

            if ((x1 < wall_x) && (wall_x < x2)) {
                if ((y1 == y2) && (std::abs(y1 - wall_y) < WALL_DETECTION_TOLERANCE)) {
                    i_min = i;
                    j_min = j;
                    minDir = 0;
                    break;
                }
            }

            x1 = grid[i][j]->south_wall_start_x;
            x2 = grid[i][j]->south_wall_end_x;
            y1 = grid[i][j]->south_wall_start_y;
            y2 = grid[i][j]->south_wall_end_y;

            if ((x1 < wall_x) && (wall_x < x2)) {
                if ((y1 == y2) && (std::abs(y1 - wall_y) < WALL_DETECTION_TOLERANCE)) {
                    i_min = i;
                    j_min = j;
                    minDir = 1;
                    break;
                }
            }

            x1 = grid[i][j]->east_wall_start_x;
            x2 = grid[i][j]->east_wall_end_x;
            y1 = grid[i][j]->east_wall_start_y;
            y2 = grid[i][j]->east_wall_end_y;


            if ((y1 < wall_y) && (wall_y < y2)) {
                if ((x1 == x2) && (std::abs(x1 - wall_x) < WALL_DETECTION_TOLERANCE)) {
                    i_min = i;
                    j_min = j;
                    minDir = 2;
                    break;
                }
            }

            x1 = grid[i][j]->west_wall_start_x;
            x2 = grid[i][j]->west_wall_end_x;
            y1 = grid[i][j]->west_wall_start_y;
            y2 = grid[i][j]->west_wall_end_y;

            if ((y1 < wall_y) && (wall_y < y2)) {
                if ((x1 == x2) && (std::abs(x1 - wall_x) < WALL_DETECTION_TOLERANCE)) {
                    i_min = i;
                    j_min = j;
                    minDir = 3;
                    break;
                }
            }
        }
    }

    if (minDir > -1){
        switch(minDir)
        {
        case(0):
            if(this->is_cell_present(grid[i_min][j_min + 1])) {
                grid[i_min][j_min] -> removeNeighborCell(grid[i_min][j_min + 1]);
                grid[i_min][j_min + 1] -> removeNeighborCell(grid[i_min][j_min]);
            }
            break;

        case(1):
            if(this->is_cell_present(grid[i_min][j_min - 1])) {
                grid[i_min][j_min] -> removeNeighborCell(grid[i_min][j_min - 1]);
                grid[i_min][j_min - 1] -> removeNeighborCell(grid[i_min][j_min]);
            }
            break;

        case(2):
            if(this->is_cell_present(grid[i_min + 1][j_min])) {
                grid[i_min][j_min] -> removeNeighborCell(grid[i_min + 1][j_min]);
                grid[i_min + 1][j_min] -> removeNeighborCell(grid[i_min][j_min]);
            }
            break;

        case(3):
            if(this->is_cell_present(grid[i_min - 1][j_min])) {
                grid[i_min][j_min] -> removeNeighborCell(grid[i_min - 1][j_min]);
                grid[i_min - 1][j_min] -> removeNeighborCell(grid[i_min][j_min]);
            }
            break;
        }
    }

}

bool Maze::is_cell_present(cell* cell_1)
{
    for (int i = 0; i < GRID_ROWS; ++i) {
        for (int j = 0; j < GRID_COLS; ++j) {
            if (grid[i][j] == cell_1)
            {
                return true;
            }
        }
    }
    return false;
}

