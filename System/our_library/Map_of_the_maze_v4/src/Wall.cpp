#include "wall.h"
#include "cell.h"
#include <iostream>
#include <cmath>

// Default constructor
wall::wall() : x1(0), y1(0), x2(0), y2(0), cell_1(nullptr), cell_2(nullptr) {
    // You can initialize the member variables here if needed
}

// Destructor
wall::~wall() {
    // If the wall is responsible for managing the lifetime of cell_1 and cell_2,
    // you should delete them here. Otherwise, this should be left empty.
    delete cell_1;
    delete cell_2;
}

// Function to calculate and print the area of a triangle formed by points (x1, y1), (x2, y2), and (x3, y3)
float wall::area_of_triangle(float x3, float y3) {
    // Using the formula for the area of a triangle given its vertices:
    // Area = 0.5 * | x1(y2 - y3) + x2(y3 - y1) + x3(y1 - y2) |
    float area = std::abs(x1*(y2 - y3) + x2*(y3 - y1) + x3*(y1 - y2)) / 2.0f;
    //std::cout << "Area of the triangle is: " << area << std::endl;
    return area;
}

// Setters for coordinates
void wall::setX1(float x) {
    x1 = x;
}

float wall::getX1() const {
    return x1;
}

void wall::setY1(float y) {
    y1 = y;
}

float wall::getY1() const {
    return y1;
}

void wall::setX2(float x) {
    x2 = x;
}

float wall::getX2() const {
    return x2;
}

void wall::setY2(float y) {
    y2 = y;
}

float wall::getY2() const {
    return y2;
}

// Setters and getters for cell pointers
void wall::setCell(cell* c1,cell* c2) {
    cell_1 = c1;
    cell_2 = c2;
}

cell* wall::getCell1() const {
    return cell_1;
}

cell* wall::getCell2() const {
    return cell_2;
}
