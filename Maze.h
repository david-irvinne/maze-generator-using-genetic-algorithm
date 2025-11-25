#include<iostream>
#include<vector>
#include <cstdlib>
#include <array>
#include <queue>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cassert>
#include <stack>
#include <set>
#include <random>
#include <chrono>

class Maze {
public:
  int ROW, COL;
  int number_of_different_path;
  double fitness_value = -1.0;
  Maze();
  Maze(int row, int col); // maze dari row dan column baru
  Maze(std::vector<short>source, int row, int col); // maze dari flatten array

  void print();
  void print_config();
  void fill_with_random_config();
  void normalize();
  void repair();
  void update_fitness_value();
  int get_min_distance();
  double fitness();
  double fitness2();
  std::vector<short> get_flatten_config();

private:
  std::vector<std::vector<short>> grid;

  bool has_left_wall(short conf);
  bool has_top_wall(short conf);
  bool has_right_wall(short conf);
  bool has_bot_wall(short conf);

  void build_left_wall(short&cell);
  void build_top_wall(short&cell);
  void build_right_wall(short&cell);
  void build_bot_wall(short&cell);

  void remove_left_wall(short&cell);
  void remove_top_wall(short&cell);
  void remove_right_wall(short&cell);
  void remove_bot_wall(short&cell);


};
