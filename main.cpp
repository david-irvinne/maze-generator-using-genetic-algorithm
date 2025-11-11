#include "Util.h"
#include<ctime>

const int ROW_SIZE = 12, COL_SIZE = 12;
const int INIT_POPULATION_SIZE = 16;

int main() {
  // buat random bisa dipakai berkali kali
  std::srand(static_cast<unsigned>(std::time(nullptr)));

  std::vector<Maze> populasi(INIT_POPULATION_SIZE); 
  int idx = 0;
  for(auto& individu: populasi){
    individu = Maze(ROW_SIZE, COL_SIZE);
    std::cout << "fitness value individu " << ++idx << ": " << individu.fitness_value << '\n';
  }
  std::cout << "\n\n";

  std::vector<Maze> new_populasi;
  while(new_populasi.size() < populasi.size()){
    Maze& parent1 = Util::tournament_selection(populasi);
    Maze& parent2 = Util::tournament_selection(populasi);

    auto[child1, child2] = Util::simple_arithmetic_crossover(parent1.get_flatten_config(), parent2.get_flatten_config());

    // mutate here

    // mutate ends here

    new_populasi.push_back(Maze(child1, ROW_SIZE, COL_SIZE));
    if(new_populasi.size() < populasi.size()){
      new_populasi.push_back(Maze(child1, ROW_SIZE, COL_SIZE));
    }
  }

  Maze& best_maze = Util::get_best_maze(populasi);
  best_maze.print();
  std::cout << best_maze.fitness_value << '\n';
    
  return 0;
}
