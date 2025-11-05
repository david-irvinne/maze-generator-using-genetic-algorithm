#include "Util.h"
#include "Maze.h"
#include<ctime>

const int ROW_SIZE = 8, COL_SIZE = 8;

int main() {
  // buat random bisa dipakai berkali kali
  std::srand(static_cast<unsigned>(std::time(nullptr)));

  Maze maze = Maze(ROW_SIZE, COL_SIZE); 
  maze.fill_with_random_config();
  maze.print();
  std::cout << maze.get_min_distance() << '\n';
  std::cout << "\n";

  Maze maze2 = Maze(ROW_SIZE, COL_SIZE);
  maze2.fill_with_random_config();
  maze2.print();
  std::cout << maze2.get_min_distance() << '\n';
  std::cout << '\n';
    
  Util GA; 
  auto[child1, child2] = GA.simple_arithmetic_crossover(maze.get_flatten_config(), maze2.get_flatten_config());

  Maze maze3 = Maze(child1, ROW_SIZE, COL_SIZE);
  Maze maze4 = Maze(child2, ROW_SIZE, COL_SIZE);

  maze3.print();
  // std::cout << maze3.get_min_distance() << '\n';
  // std::cout << "\n";

  maze4.print();
  // std::cout << maze4.get_min_distance() << '\n';
  // std::cout << "\n";
    
  return 0;
}
