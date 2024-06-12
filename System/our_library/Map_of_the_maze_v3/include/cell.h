#ifndef CELL_H
#define CELL_H

#include <vector>

class cell {
public:
    cell(int x, int y);
    ~cell();

    void setX(int x);
    int getX() const;

    void setY(int y);
    int getY() const;

    void setValue(int value);
    int getValue() const;

    void setwalls();
    void addNeighborCell(cell* Neighbor);
    const std::vector<cell*>& getNeighborCells() const;
    void removeNeighborCell(cell* Neighbor);

    float north_wall_start_x,north_wall_start_y,north_wall_end_x,north_wall_end_y;
    float south_wall_start_x,south_wall_start_y,south_wall_end_x,south_wall_end_y;
    float east_wall_start_x,east_wall_start_y,east_wall_end_x,east_wall_end_y;
    float west_wall_start_x,west_wall_start_y,west_wall_end_x,west_wall_end_y;

private:
    int x;
    int y;
    int value;
    std::vector<cell*> NeighborCells;

};

#endif // CELL_H
