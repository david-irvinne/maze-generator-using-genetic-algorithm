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


