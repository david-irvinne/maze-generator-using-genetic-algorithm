#include "Util.h"
#include <iostream>
#include<vector>

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
    std::cout << "this is k: " << K << '\n';
    std::cout << "this is a: " << a << '\n';
    std::cout << "this is sz: " << sz << '\n';
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



