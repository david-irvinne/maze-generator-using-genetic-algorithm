#include "util.h"
#include "Maze.h"

int main() {
    greet("David");

    Maze maze = Maze(7, 7); 
    maze.fill_with_random_config();

    maze.print();
    
    return 0;
}
