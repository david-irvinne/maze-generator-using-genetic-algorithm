#include "Util.h"
#include "Maze.h"

int main() {
    Util util;
    util.greet("David");

    Maze maze = Maze(7, 7); 
    maze.fill_with_random_config();
    maze.print();
    std::cout << maze.get_min_distance() << '\n';
    
    return 0;
}
