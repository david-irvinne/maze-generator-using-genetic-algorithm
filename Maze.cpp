#include "Maze.h"
#include <cstdlib>
#include <ctime>
#include <array>
#include <queue>

#define dbg(x) std::cout << "["<< #x <<"] : "<< (x) <<std::endl;

Maze::Maze(int row, int col){
  ROW = row;
  COL = col;
  grid = std::vector<std::vector<short>>(ROW,  std::vector<short>(COL));
}

Maze::Maze(std::vector<short>source, int row, int col){
  ROW = row;
  COL = col;
  grid = std::vector<std::vector<short>>(ROW, std::vector<short>(COL));
  for(int i = 0, k = 0; i < ROW; i++){
    for(int j = 0; j < COL; j++){
      grid[i][j] = source[k];
      k++;
    }
  }

  normalize();
  repair();
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
    }
  }
  normalize();
  repair();
  
  // selagi masih ga solvable, isi dengan config random baru
  while(get_min_distance() == -1) fill_with_random_config();
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

      // jika ini kolom paling kiri, bangung dinding kiri
      if(j == 0) build_left_wall(grid[i][j]);
      // jika ini baris paling atas, bangun dinding atas
      if(i == 0) build_top_wall(grid[i][j]);
      // jika ini kolom paling kanan, bangung dinding kanan
      if(j == COL-1) build_right_wall(grid[i][j]);
      // jika ini baris paling bawah, bangun dinding bawah
      if(i == ROW-1) build_bot_wall(grid[i][j]);

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

int Maze::get_min_distance(){
  const int INF = 1e9;
  std::vector<std::vector<int>> dist(ROW, std::vector<int>(COL, INF));
  std::vector<std::vector<bool>> vis(ROW, std::vector<bool>(COL));

  dist[0][0] = 0;
  std::queue<std::array<int, 2>> q;
  q.push({0, 0});
  while(!q.empty()){
    auto[now_r, now_c] = q.front();
    q.pop();
    if(vis[now_r][now_c]) continue;

    vis[now_r][now_c] = true;
    // coba pergi ke atas
    if(!has_top_wall(grid[now_r][now_c])) {
      q.push({now_r - 1, now_c});
      dist[now_r - 1][now_c] = std::min(dist[now_r - 1][now_c], dist[now_r][now_c] + 1);
    }
    // coba pergi ke bawah
    if(!has_bot_wall(grid[now_r][now_c])) {
      q.push({now_r + 1, now_c});
      dist[now_r + 1][now_c] = std::min(dist[now_r + 1][now_c], dist[now_r][now_c] + 1);
    }
    // coba pergi ke kanan
    if(!has_right_wall(grid[now_r][now_c])) {
      q.push({now_r , now_c + 1});
      dist[now_r][now_c + 1] = std::min(dist[now_r][now_c + 1], dist[now_r][now_c] + 1);
    }
    // coba pergi ke kiri 
    if(!has_left_wall(grid[now_r][now_c])) {
      q.push({now_r , now_c - 1});
      dist[now_r][now_c - 1] = std::min(dist[now_r][now_c - 1], dist[now_r][now_c] + 1);
    }
  }

  return (dist[ROW-1][COL-1] == INF ? -1 : dist[ROW-1][COL-1]);
}

// Fitness by "human-style" DFS with direction bias and crumbs.
// Start: (0,0), Goal: (ROW-1,COL-1). Never returns -1.
int Maze::fitness() {
  // trivial guard
  if (ROW == 0 || COL == 0) return 0;

  // 0=Up, 1=Right, 2=Down, 3=Left
  const int dr[4] = {-1, 0, 1, 0};
  const int dc[4] = { 0, 1, 0,-1};

  auto in_bounds = [&](int r, int c) {
    return r >= 0 && r < ROW && c >= 0 && c < COL;
  };

  // Check move using your wall-bit helpers (they take a short cell conf)
  auto can_move = [&](int r, int c, int d) {
    short cell = grid[r][c];
    bool blocked = false;
    if (d == 0) blocked = has_top_wall(cell);
    if (d == 1) blocked = has_right_wall(cell);
    if (d == 2) blocked = has_bot_wall(cell);
    if (d == 3) blocked = has_left_wall(cell);
    if (blocked) return false;
    int nr = r + dr[d], nc = c + dc[d];
    return in_bounds(nr, nc);
  };

  // visited & crumbs
  std::vector<std::vector<bool>> vis(ROW, std::vector<bool>(COL, false));
  std::vector<std::vector<int>>  crumbs(ROW, std::vector<int>(COL, 0));

  struct Node { int r, c, d, tried; };
  std::vector<Node> st;
  st.reserve(ROW * COL);

  auto left_of    = [](int d){ return (d + 3) & 3; };
  auto right_of   = [](int d){ return (d + 1) & 3; };
  auto back_of    = [](int d){ return (d + 2) & 3; };
  auto forward_of = [](int d){ return d; };

  // initial heading: first open direction around start (0,0), else 0
  int d0 = 0;
  for (int d = 0; d < 4; ++d) if (can_move(0, 0, d)) { d0 = d; break; }

  st.push_back({0, 0, d0, 0});
  vis[0][0] = true;
  crumbs[0][0] += 1;

  auto bias_order = [&](int d) {
    // 3 simple bias variants; pick one randomly each step
    int r = std::rand() % 3;
    if (r == 0) return std::array<int,4>{ forward_of(d), left_of(d),  right_of(d), back_of(d) };
    if (r == 1) return std::array<int,4>{ left_of(d),    forward_of(d), right_of(d), back_of(d) };
    return                 std::array<int,4>{ right_of(d),   forward_of(d), left_of(d),  back_of(d) };
  };

  const int GR = ROW - 1, GC = COL - 1;

  while (!st.empty()) {
    auto &top = st.back();
    int r = top.r, c = top.c, d = top.d;

    if (r == GR && c == GC) {
      // number of moves = edges along the stack path
      return (int)st.size() - 1;
    }

    auto order = bias_order(d);
    bool moved = false;

    for (int k = top.tried; k < 4; ++k) {
      int nd = order[k];
      int nr = r + dr[nd], nc = c + dc[nd];

      if (!can_move(r, c, nd))   continue;
      if (crumbs[nr][nc] >= 2)   continue;   // avoid over-revisiting

      top.tried = k + 1;                       // we have tried up to k
      st.push_back({nr, nc, nd, 0});
      if (!vis[nr][nc]) vis[nr][nc] = true;
      crumbs[nr][nc] += 1;
      moved = true;
      break;
    }

    if (!moved) st.pop_back();                 // backtrack
  }

  // You said maze is always reachable; fallback (shouldn’t happen).
  return (int)(1e9);
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

std::vector<short> Maze::get_flatten_config() {
  std::vector<short> ret;
  for(int i = 0; i < ROW; i++){
    for(int j = 0; j < COL; j++) ret.push_back(grid[i][j]);
  }
  return ret;
}
