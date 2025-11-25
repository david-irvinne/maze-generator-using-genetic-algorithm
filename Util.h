#include<string>
#include<vector>
#include <stack>
#include <set>
#include <random>
#include <chrono>
#include "Maze.h"

class Util{
public :
  void greet(std::string name);
  static Maze& tournament_selection(std::vector<Maze>& populasi, int k = 1); 
  static Maze& roulette_selection(std::vector<Maze>& populasi) ;
  static Maze& get_best_maze(std::vector<Maze>& populasi);
  static bool check_convergence(std::vector<Maze>& populasi);

  static std::pair<std::vector<short>, std::vector<short>> simple_arithmetic_crossover(
    std::vector<short>parent1, 
    std::vector<short>parent2
  );

  static std::pair<std::vector<short>, std::vector<short>> uniform_biased_crossover(
    std::vector<short> parent1,
    std::vector<short> parent2
  );
  static void mutate_bitflip(
    std::vector<short>& chrom,
    double p_cell = 0.02,
    double p_big_mut = 0.1
  );

private:

};


