#include "Util.h"
#include <thread>
#include <chrono>
#include<ctime>

#define dbg(x) std::cout << "["<< #x <<"] : "<< (x) <<std::endl;

const int ROW_SIZE = 12, COL_SIZE = 12;
const int INIT_POPULATION_SIZE = 32;
const int MAX_GENERATION = 16;

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
  for(int GEN = 0; GEN < MAX_GENERATION; GEN++){
    while(new_populasi.size() < populasi.size()){
      // Maze& parent1 = Util::tournament_selection(populasi, INIT_POPULATION_SIZE / 10);
      // Maze& parent2 = Util::tournament_selection(populasi, INIT_POPULATION_SIZE / 10);
      Maze& parent1 = Util::roulette_selection(populasi);
      Maze& parent2 = Util::roulette_selection(populasi);

      // auto[child1, child2] = Util::simple_arithmetic_crossover(parent1.get_flatten_config(), parent2.get_flatten_config());
      auto[child1, child2] = Util::uniform_biased_crossover(parent1.get_flatten_config(), parent2.get_flatten_config());

      // mutate here
      Util::mutate_bitflip(child1, 0.02, 0.1);
      Util::mutate_bitflip(child2, 0.02, 0.1);
      // mutate ends here

      new_populasi.push_back(Maze(child1, ROW_SIZE, COL_SIZE));
      if (new_populasi.size() < populasi.size()) {
          new_populasi.push_back(Maze(child2, ROW_SIZE, COL_SIZE));
      }
    }
    
    populasi = new_populasi;
    new_populasi.clear();

    bool all_converges = Util::check_convergence(populasi);
    dbg(all_converges);
    if(all_converges) break;

    // output hasil terbaik dari gen sekarang
    std::cout << "hasil terbaik dari gen " << GEN << ":\n";
    Maze& best_maze = Util::get_best_maze(populasi);
    best_maze.print();
    std::cout << "BEST FITNESS: " << best_maze.fitness_value << "\n\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

    
  return 0;
}
