#include "Util.h"
#include <thread>
#include <chrono>
#include<ctime>

#define dbg(x) std::cout << "["<< #x <<"] : "<< (x) <<std::endl;

const int ROW_SIZE = 12, COL_SIZE = 12;
const int INIT_POPULATION_SIZE = 64;
const int MAX_GENERATION = 32;
const int ELITISM_SIZE = 2; 

int main() {
  // buat random bisa dipakai berkali kali
  std::srand(static_cast<unsigned>(std::time(nullptr)));

  std::vector<Maze> populasi(INIT_POPULATION_SIZE); 
  Util::fill_population(populasi, ROW_SIZE, COL_SIZE);

  std::vector<Maze> new_populasi;
  for(int GEN = 0; GEN < MAX_GENERATION; GEN++){
    while(new_populasi.size() < populasi.size()){
      // SELECTION HERE
      Maze& parent1 = Util::roulette_selection(populasi);
      Maze& parent2 = Util::roulette_selection(populasi);
      // Maze& parent1 = Util::tournament_selection(populasi, INIT_POPULATION_SIZE / 2);
      // Maze& parent2 = Util::tournament_selection(populasi, INIT_POPULATION_SIZE / 2);
      // SELECTION ENDS HERE
      
      // CROSSOVER HERE 
      auto[child1, child2] = Util::uniform_biased_crossover(parent1.get_flatten_config(), parent2.get_flatten_config());
      // auto[child1, child2] = Util::simple_arithmetic_crossover(parent1.get_flatten_config(), parent2.get_flatten_config());
      // CROSSOVER ENDS HERE 

      // MUTATION HERE 
      Util::mutate_bitflip(child1, 0.02, 0.1);
      Util::mutate_bitflip(child2, 0.02, 0.1);
      // MUTATION ENDS HERE 

      new_populasi.push_back(Maze(child1, ROW_SIZE, COL_SIZE));
      if (new_populasi.size() < populasi.size()) {
          new_populasi.push_back(Maze(child2, ROW_SIZE, COL_SIZE));
      }
    }
    
    // ELITISM, klo size nya 0 jadi update populasi biasa
    Util::elitism(ELITISM_SIZE, populasi, new_populasi);

    // output hasil terbaik dari gen sekarang
    Util::print_current_gen_summary(populasi, GEN);

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  Util::print_summary();
    
  return 0;
}
