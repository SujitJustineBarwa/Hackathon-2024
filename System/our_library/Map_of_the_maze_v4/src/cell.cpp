#include "cell.h"
#include "constants.h"
#include <bits/stdc++.h>

// Constructor
cell::cell(int x, int y) : x(x), y(y), value(0) {
    // ctor
}

// Destructor
cell::~cell() {
    // dtor
    for (cell* c : NeighborCells) {
        delete c;  // Assuming ownership, otherwise omit this line
    }
}

// Setter for x
void cell::setX(int x) {
    this->x = x;
}

// Getter for x
int cell::getX() const {
    return x;
}

// Setter for y
void cell::setY(int y) {
    this->y = y;
}

// Getter for y
int cell::getY() const {
    return y;
}

// Setter for value
void cell::setValue(int value) {
    this->value = value;
}

// Getter for value
int cell::getValue() const {
    return value;
}

// Wall settings
void cell::setwalls(){
    this->north_wall_start_x = (float)this->getX() * WALL_SIZE;
    this->north_wall_start_y = (float)(this->getY() + 1) * WALL_SIZE;
    this->north_wall_end_x = (float)(this->getX() + 1) * WALL_SIZE;
    this->north_wall_end_y = (float)(this->getY() + 1) * WALL_SIZE;

    this->south_wall_start_x = (float)this->getX() * WALL_SIZE;
    this->south_wall_start_y = (float)this->getY() * WALL_SIZE;
    this->south_wall_end_x = (float)(this->getX() + 1) * WALL_SIZE;
    this->south_wall_end_y = (float)this->getY() * WALL_SIZE;

    this->east_wall_start_x = (float)(this->getX() + 1) * WALL_SIZE;
    this->east_wall_start_y = (float)this->getY() * WALL_SIZE;
    this->east_wall_end_x = (float)(this->getX() + 1) * WALL_SIZE;
    this->east_wall_end_y = (float)(this->getY() + 1) * WALL_SIZE;

    this->west_wall_start_x = (float)this->getX() * WALL_SIZE;
    this->west_wall_start_y = (float)this->getY() * WALL_SIZE;
    this->west_wall_end_x = (float)this->getX() * WALL_SIZE;
    this->west_wall_end_y = (float)(this->getY() + 1) * WALL_SIZE;
}

// Add a cell to NeighborCells
void cell::addNeighborCell(cell* Neighbor) {
    NeighborCells.push_back(Neighbor);
}

void cell::removeNeighborCell(cell* Neighbor) {

    auto it = find(this->NeighborCells.begin(), this->NeighborCells.end(),Neighbor);

    if (it != this->NeighborCells.end()) {
        this->NeighborCells.erase(it);
    }
}

// Get all Neighbor cells
const std::vector<cell*>& cell::getNeighborCells() const {
    return NeighborCells;
}
