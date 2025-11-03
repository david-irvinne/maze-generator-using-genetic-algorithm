#include "Maze.h"
#include <cstdlib>
#include <ctime>

#define dbg(x) std::cout << "["<< #x <<"] : "<< (x) <<std::endl;

Maze::Maze(int row, int col){
  ROW = row;
  COL = col;
  grid = std::vector<std::vector<short>>(ROW,  std::vector<short>(COL));
  // buat random bisa dipakai berkali kali
  std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void Maze::print_config(){
  std::cout << "ROW: " << ROW << ' ' << "COL: " << COL << '\n';
  for(int i = 0; i < ROW; i++){
    for(int j = 0; j < COL; j++) std::cout << grid[i][j] << ' ';
    std::cout << '\n';
  }
}

void Maze::print(){
  if (ROW == 0) return;

  // Baris paling atas (dinding atas semua cell)
  std::cout << "+";
  for (int j = 0; j < COL; ++j) {
    std::cout << (grid[0][j] & 2 ? "---" : "   ") << "+";
  }
  std::cout << '\n';

  // Untuk setiap baris
  for (int i = 0; i < ROW; ++i) {
    std::cout << "|"; 
    for (int j = 0; j < COL; ++j) {
      std::cout << "   "; 
      if (grid[i][j] & 4) {
        std::cout << "|";
      } else {
        std::cout << " ";
      }
    }
    std::cout << '\n';

    // Baris bawah: dinding bawah
    std::cout << "+";
    for (int j = 0; j < COL; ++j) {
      if (grid[i][j] & 8) {
        std::cout << "---";
      } else {
        std::cout << "   ";
      }
      std::cout << "+";
    }
    std::cout << '\n';
  }
}

void Maze::fill_with_random_config(){
  for(int i = 0; i < ROW; i++){
    for(int j = 0; j < COL; j++){
      grid[i][j] = std::rand() % 16; 

      // jika ini kolom paling kiri, bangung dinding kiri
      if(j == 0) build_left_wall(grid[i][j]);
      // jika ini baris paling atas, bangun dinding atas
      if(i == 0) build_top_wall(grid[i][j]);
      // jika ini kolom paling kanan, bangung dinding kanan
      if(j == COL-1) build_right_wall(grid[i][j]);
      // jika ini baris paling bawah, bangun dinding bawah
      if(i == ROW-1) build_bot_wall(grid[i][j]);
    }
  }

  normalize();
  repair();
}

// pastikan grid yang terbentuk itu konsisten
void Maze::normalize(){
  for(int i = 0; i < ROW; i++){
    for(int j = 0; j < COL; j++){

      // kalau punya tetangga kanan
      if(j + 1 < COL){
        // kalau di sini punya dinding kanan, maka tambahin dinding kiri di next cell
        if(has_right_wall(grid[i][j])) build_left_wall(grid[i][j+1]);

        // kalau di sini ga punya dinding kanan, maka hapus dinding kiri di next cell
        else remove_left_wall(grid[i][j + 1]); 
      }

      // kalau punya tetangga bawah
      if(i + 1 < ROW){
        // kalau di sini punya dinding bawah, maka tambahin dinding atas di below cell
        if(has_bot_wall(grid[i][j])) build_top_wall(grid[i+1][j]);

        // kalau di sini ga punya dinding bawah, maka hapus dinding atas di below cell
        else remove_top_wall(grid[i+1][j]);
      }
    }
  } 
}

void Maze::repair(){
  for(int i = 0; i < ROW; i++){
    for(int j = 0; j < COL; j++){
      if(grid[i][j] != 15) continue;

      std::vector<short> candidates; // kandidat dinding untuk dibuka
      if(j > 0) candidates.push_back(1);          // kiri
      if(i > 0) candidates.push_back(2);          // atas 
      if(j + 1 < COL) candidates.push_back(4);    // kanan 
      if(i + 1 < ROW) candidates.push_back(8);    // bawah 
      
      // pilih salah satu sisi, lalu hapus dindingnya
      short wall_to_remove = candidates[std::rand() % (int)candidates.size()];
      grid[i][j] ^= wall_to_remove; 
      
      // perbaiki juga dinding tetangganya
      if(wall_to_remove == 1) remove_right_wall(grid[i][j-1]);
      if(wall_to_remove == 2) remove_bot_wall(grid[i-1][j]);
      if(wall_to_remove == 4) remove_left_wall(grid[i][j+1]);
      if(wall_to_remove == 8) remove_top_wall(grid[i+1][j]);
    }
  }
}

bool Maze::has_solution(){
  return true;
}

// {1, 2, 4, 8} -> {kiri, atas, kanan, bawah}
bool Maze::has_left_wall(short conf){
  return (conf >> 0) & 1; 
}    
bool Maze::has_top_wall(short conf){
  return (conf >> 1) & 1; 
}    
bool Maze::has_right_wall(short conf){
  return (conf >> 2) & 1; 
}    
bool Maze::has_bot_wall(short conf){
  return (conf >> 3) & 1; 
}   

// {1, 2, 4, 8} -> {kiri, atas, kanan, bawah}
void Maze::build_left_wall(short&cell){
  cell |= 1;
};
void Maze::build_top_wall(short&cell){
  cell |= 2;
};
void Maze::build_right_wall(short&cell){
  cell |= 4;
};
void Maze::build_bot_wall(short&cell){
  cell |= 8;
};

// {1, 2, 4, 8} -> {kiri, atas, kanan, bawah}
void Maze::remove_left_wall(short&cell){
  if(has_left_wall(cell)) cell ^= 1;
};
void Maze::remove_top_wall(short&cell){
  if(has_top_wall(cell)) cell ^= 2;
};
void Maze::remove_right_wall(short&cell){
  if(has_right_wall(cell)) cell ^= 4;
};
void Maze::remove_bot_wall(short&cell){
  if(has_bot_wall(cell)) cell ^= 8;
};
