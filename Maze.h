#include<iostream>
#include<vector>

class Maze {
public:
  Maze(int row, int col);

  void print();
  void print_config();
  void fill_with_random_config();
  void normalize();
  void repair();
  int get_min_distance();

private:
  int ROW, COL;
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
