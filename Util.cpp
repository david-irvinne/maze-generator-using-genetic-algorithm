#include "Util.h"
#include <cmath>
#include <iostream>
#include<vector>
#include<algorithm>

void Util::greet(std::string name) {
    std::cout << "Hello, " << name << "!" << std::endl;
}

std::pair<std::vector<short>, std::vector<short>> Util::simple_arithmetic_crossover(std::vector<short>parent1, std::vector<short>parent2){
  int sz = parent1.size();
  std::vector<short> child1 = parent1, child2 = parent2;

  // choose random k (the starting point of crossover)
  int K = std::rand() % sz; 
  // choose random a (factor untuk crossover)
  float a = (float)std::rand() / RAND_MAX;

  for(int i = 0; i < sz; i++){
    if(i < K) continue; // tidak ada crossover
    else {
      child1[i] = child2[i] * a + child1[i] * (1 - a); 
      child2[i] = child1[i] * a + child2[i] * (1 - a);

      child1[i] = std::min((short)15, child1[i]);
      child2[i] = std::min((short)15, child2[i]);

      child1[i] = std::max((short)1, child1[i]);
      child2[i] = std::max((short)1, child2[i]);
    }
  }


  return {child1, child2};
}

std::pair<std::vector<short>, std::vector<short>> Util::uniform_biased_crossover(
    std::vector<short> parent1,
    std::vector<short> parent2
){
    const int sz = static_cast<int>(parent1.size());
    std::vector<short> child1 = parent1;
    std::vector<short> child2 = parent2;

    // Bias toward parent1 (assume parent1 is fitter or preferred)
    const float bias = 0.70f; // tweakable

    for (int i = 0; i < sz; ++i) {
        float u = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);

        // If we take from parent1 for child1, take from parent2 for child2 (complementary)
        if (u < bias) {
            child1[i] = parent1[i];
            child2[i] = parent2[i];
        } else {
            child1[i] = parent2[i];
            child2[i] = parent1[i];
        }

        // Clamp to valid wall-mask range [1, 15]
        if (child1[i] < 1)  child1[i] = 1;
        if (child1[i] > 15) child1[i] = 15;
        if (child2[i] < 1)  child2[i] = 1;
        if (child2[i] > 15) child2[i] = 15;
    }

    return { child1, child2 };
}

Maze& Util::tournament_selection(std::vector<Maze>& populasi, int k){
  int best_idx = rand() % populasi.size();
  
  // di antara k random pilihan, cari yang terbaik
  for(int i = 1; i < k; i++){
    int idx = rand() % populasi.size();
    if(populasi[idx].fitness_value < populasi[best_idx].fitness_value){
      best_idx = idx;
    }
  }
  return populasi[best_idx];
}

Maze& Util::get_best_maze(std::vector<Maze>& populasi){
  Maze& res = populasi[0];
  for(auto& individu : populasi){
    if(individu.fitness_value < res.fitness_value) res = individu;
  }
  return res;
}

// cek kalau semua fitness value dari sebuah populasi udah sama 
bool Util::check_convergence(std::vector<Maze>& populasi){
  bool converges = true;
  for(int i = 0; i < (int)populasi.size() - 1; i++){
    converges &= populasi[i].fitness_value == populasi[i + 1].fitness_value;
  }
  return converges;
}

Maze& Util::roulette_selection(std::vector<Maze>& populasi) {
    double total = 0.0;
    for (auto& m : populasi) total += (1.0 / m.fitness_value);  // fitness kecil = bobot besar

    double r = ((double) rand() / RAND_MAX) * total;

    double cumulative = 0.0;
    for (auto& m : populasi) {
        cumulative += (1.0 / m.fitness_value);
        if (cumulative >= r) {
            return m;
        }
    }
    return populasi.back();
}


void Util::mutate_bitflip(std::vector<short>& chrom,
                          double p_cell,
                          double p_big_mut)
{
    int n = static_cast<int>(chrom.size());
    for (int i = 0; i < n; ++i) {
        double u = static_cast<double>(std::rand()) / RAND_MAX;
        if (u < p_cell) {
            // with small probability, do a "big" mutation: random wall mask
            double v = static_cast<double>(std::rand()) / RAND_MAX;
            if (v < p_big_mut) {
                chrom[i] = static_cast<short>((std::rand() % 15) + 1); // 1..15
            } else {
                // bit-flip mutation on one randomly chosen wall bit
                int bit = std::rand() % 4;            // 0..3
                chrom[i] ^= static_cast<short>(1 << bit);
                // clamp to [1,15] just in case we get 0
                if (chrom[i] < 1)  chrom[i] = 1;
                if (chrom[i] > 15) chrom[i] = 15;
            }
        }
    }
}

void Util::elitism(int ELITISM_SIZE,
                   std::vector<Maze>& populasi,
                   std::vector<Maze>& new_populasi){
  if (ELITISM_SIZE <= 0){
    populasi = new_populasi;
    new_populasi.clear();
    return;   // tidak ada elitism
  }

  std::vector<Maze> old_pop = populasi;

  std::sort(old_pop.begin(), old_pop.end(), [&](Maze a, Maze b){
    return a.fitness_value < b.fitness_value;  // ascending (terbaik di depan)
  });

  populasi = new_populasi;
  std::sort(populasi.begin(), populasi.end(), [&](Maze a, Maze b){
    return a.fitness_value > b.fitness_value;
  });

  for (int i = 0; i < std::min(ELITISM_SIZE, (int)old_pop.size()); i++) {
    populasi[i] = old_pop[i];   // replace individu terburuk
  }

  new_populasi.clear();  
}
