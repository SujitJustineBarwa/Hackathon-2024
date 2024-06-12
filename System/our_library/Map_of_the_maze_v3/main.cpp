#include "maze.h"
#include "localization.h"
#include <cmath>

int main() {
    Maze maze;
    localization loc;

    maze.floodFill(2,2);

    maze.display();
    std::cout << std::endl;
    std::cout << std::endl;
    maze.display();

    std::cout << std::endl;
    std::cout << std::endl;
    maze.find_wall_coordinates_and_update_walls(0,0,3.14/6,207.85,9000,9000,9000);
    maze.floodFill(2,2);
    maze.display();

    // Set the robot's location
    loc.setLocation(36,180,3.14/6);

    // Get and print the robot's location
    std::cout << "Robot's location: (" << loc.getX() << ", " << loc.getY() << ")" << std::endl;
    std::cout << "Robot's cell location: (" << loc.get_cell_X() << ", " << loc.get_cell_Y() << ")" << std::endl;


    std::cout << std::endl;
    std::cout << std::endl;
    maze.find_wall_coordinates_and_update_walls(0,180,3.14/6,207.85,9000,9000,9000);
    maze.floodFill(2,2);
    maze.display();

    std::cout << std::endl;
    std::cout << std::endl;
    maze.find_wall_coordinates_and_update_walls(0,360,3.14/6,207.85,9000,9000,9000);
    maze.floodFill(2,2);
    maze.display();

    return 0;
}
