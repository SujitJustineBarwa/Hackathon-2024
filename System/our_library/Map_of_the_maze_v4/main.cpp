#include "maze.h"
#include "localization.h"
#include <cmath>

int main() {
    Maze maze;

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

    maze.getWaypoints(0,0);

    std::vector<int> pathX = maze.getPathX();
    std::vector<int> pathY = maze.getPathY();

    std::cout << "Path X coordinates: ";
    for (int x : pathX) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    std::cout << "Path Y coordinates: ";
    for (int y : pathY) {
        std::cout << y << " ";
    }
    std::cout << std::endl;

    return 0;
}
