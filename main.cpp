#include "util.h"
#include "Maze.h"

int main() {
    greet("David");

    Maze mz = Maze(3, 3); 
    mz.fill_with_random_config();
    
    return 0;
}
