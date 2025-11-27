#include<string>
#include<vector>
#include <stack>
#include <set>
#include <random>
#include <chrono>
#include "Maze.h"

class Util{
public :
  static Maze& tournament_selection(std::vector<Maze>& populasi, int k = 1); 
  static Maze& roulette_selection(std::vector<Maze>& populasi) ;
  static Maze& get_best_maze(std::vector<Maze>& populasi);
  static bool check_convergence(std::vector<Maze>& populasi);
  static void fill_population(std::vector<Maze>& populasi, int ROW_SIZE, int COL_SIZE);

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

  static void elitism(int ELITISM_SIZE,
                    std::vector<Maze>& populasi,
                    std::vector<Maze>& new_populasi);
  static void print_current_gen_summary(std::vector<Maze>&population, int GEN);

  static void print_summary();

  static std::vector<double> fitness_summary;
  static std::vector<int> path_number_summary;
  static std::vector<double>avg_step_summary;
private:

};


