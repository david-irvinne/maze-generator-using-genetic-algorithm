#include "Util.h"
#include "Maze.h"
#include<ctime>

const int ROW_SIZE = 12, COL_SIZE = 12;
const int INIT_POPULATION_SIZE = 10;

int main() {
  // buat random bisa dipakai berkali kali
  std::srand(static_cast<unsigned>(std::time(nullptr)));

  std::vector<Maze> populasi(INIT_POPULATION_SIZE); 
  for(auto& individu: populasi){
    individu = Maze(ROW_SIZE, COL_SIZE);
  }
  for(auto m : populasi){
    m.print();
    std::cout << m.get_min_distance() << '\n';
    std::cout << m.fitness() << '\n';
  }

    
  return 0;
}
