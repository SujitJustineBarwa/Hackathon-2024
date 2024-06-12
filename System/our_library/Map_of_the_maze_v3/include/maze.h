#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <iostream>
#include <queue>
#include "cell.h"
#include "Wall.h"
#include "constants.h"

class Maze {
private:
    std::vector<std::vector<cell*>> grid;
    std::vector<wall*> walls;

public:
    Maze();
    ~Maze();
    void display() const;
    void floodFill(int startX, int startY);
    void update_cell_neigbours();
    void find_wall_coordinates_and_update_walls(float x, float y, float theta, float front_TOF_reading, float back_TOF_reading,float left_TOF_reading,float right_TOF_reading);
    void wall_update(float wall_x,float wall_y);
    bool is_cell_present(cell* cell_1);

private:
    void createGrid();
    void setNeighbors();
    void cleanUp();
};

#endif // MAZE_H
