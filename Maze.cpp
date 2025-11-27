#include "Maze.h"

#define dbg(x) std::cout << "["<< #x <<"] : "<< (x) <<std::endl;

Maze::Maze() {
  ROW = 0;
  COL = 0;
  number_of_different_path = 0;
  avg_steps_taken = 0;
}

void Maze::update_fitness_value(){
  fitness_value = fitness_dfs();
}

Maze::Maze(int row, int col){
  ROW = row;
  COL = col;
  grid = std::vector<std::vector<short>>(ROW,  std::vector<short>(COL));
  number_of_different_path = 0;
  avg_steps_taken = 0;
  
  // isi dengan random config yang punya solusi
  fill_with_random_config();
  normalize();
  repair();
  
  // selagi masih ga solvable, isi dengan config random baru
  while(get_min_distance() == -1) fill_with_random_config();

  update_fitness_value();
}

Maze::Maze(std::vector<short>source, int row, int col){
  ROW = row;
  COL = col;
  grid = std::vector<std::vector<short>>(ROW, std::vector<short>(COL));
  number_of_different_path = 0;
  avg_steps_taken = 0;
  for(int i = 0, k = 0; i < ROW; i++){
    for(int j = 0; j < COL; j++){
      grid[i][j] = source[k];
      k++;
    }
  }

  normalize();
  repair();
  update_fitness_value();
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

    // normal BFS distance
    std::vector<std::vector<int>> dist(ROW, std::vector<int>(COL, INF));

    // tambahan untuk menghitung banyak jalur
    std::vector<std::vector<int>> ways(ROW, std::vector<int>(COL, 0));

    std::vector<std::vector<bool>> vis(ROW, std::vector<bool>(COL, false));

    dist[0][0] = 0;
    ways[0][0] = 1; // hanya 1 cara untuk memulai dari start

    std::queue<std::array<int, 2>> q;
    q.push({0, 0});

    while(!q.empty()){
        auto [now_r, now_c] = q.front();
        q.pop();

        if(vis[now_r][now_c]) continue;
        vis[now_r][now_c] = true;

        auto try_move = [&](int next_r, int next_c){
            if(next_r < 0 || next_r >= ROW || next_c < 0 || next_c >= COL) return; // out of bounds

            if(dist[next_r][next_c] > dist[now_r][now_c] + 1){
                dist[next_r][next_c] = dist[now_r][now_c] + 1;
                ways[next_r][next_c] = ways[now_r][now_c]; // copy jumlah cara
                q.push({next_r, next_c});
            }
            // jika jaraknya sama → berarti ada jalur alternatif ke sana
            else if(dist[next_r][next_c] == dist[now_r][now_c] + 1){
                ways[next_r][next_c] += ways[now_r][now_c];
            }
        };

        if(!has_top_wall(grid[now_r][now_c]))   try_move(now_r - 1, now_c);
        if(!has_bot_wall(grid[now_r][now_c]))   try_move(now_r + 1, now_c);
        if(!has_right_wall(grid[now_r][now_c])) try_move(now_r, now_c + 1);
        if(!has_left_wall(grid[now_r][now_c]))  try_move(now_r, now_c - 1);
    }

    number_of_different_path = ways[ROW-1][COL-1];

    return (dist[ROW-1][COL-1] == INF ? -1 : dist[ROW-1][COL-1]);
}

double Maze::fitness_bfs() {
  if (ROW == 0 || COL == 0) return 1;    // not shaped
  if(get_min_distance() == -1) return 1; // no solution

  const int SR = 0, SC = 0, GR = ROW - 1, GC = COL - 1;
  const int dr[4] = {-1, 0, 1, 0};
  const int dc[4] = { 0, 1, 0,-1};

  auto in_bounds = [&](int r, int c){ return r>=0 && r<ROW && c>=0 && c<COL; };

  auto can_move_from = [&](int r, int c, int d){
    short cell = grid[r][c];
    if (d==0 && has_top_wall(cell))   return false;
    if (d==1 && has_right_wall(cell)) return false;
    if (d==2 && has_bot_wall(cell))   return false;
    if (d==3 && has_left_wall(cell))  return false;
    int nr = r + dr[d], nc = c + dc[d];
    return in_bounds(nr, nc);
  };

  auto neighbors = [&](int r, int c, std::array<std::pair<int,int>,4>& out){
    int m = 0;
    for (int d=0; d<4; ++d) if (can_move_from(r,c,d)) {
      out[m++] = {r+dr[d], c+dc[d]};
    }
    return m; // count
  };

  auto degree_rc = [&](int r, int c){
    int deg = 0;
    short cell = grid[r][c];
    if (!has_top_wall(cell)   && in_bounds(r-1,c)) ++deg;
    if (!has_right_wall(cell) && in_bounds(r,c+1)) ++deg;
    if (!has_bot_wall(cell)   && in_bounds(r+1,c)) ++deg;
    if (!has_left_wall(cell)  && in_bounds(r,c-1)) ++deg;
    return deg;
  };

  // -------------------------------
  // 1) BFS distances + #shortest paths
  // -------------------------------
  const int INF = 1e9;
  std::vector<std::vector<int>> dist(ROW, std::vector<int>(COL, INF));
  std::vector<std::vector<long long>> ways(ROW, std::vector<long long>(COL, 0));
  std::vector<std::vector<std::pair<int,int>>> parent(ROW, std::vector<std::pair<int,int>>(COL, {-1,-1}));
  std::queue<std::pair<int,int>> q;

  dist[SR][SC] = 0; ways[SR][SC] = 1;
  q.push({SR,SC});

  std::array<std::pair<int,int>,4> buff;
  while(!q.empty()){
    auto [r,c] = q.front(); q.pop();
    int cnt = neighbors(r,c,buff);
    for(int i=0;i<cnt;i++){
      auto [nr,nc] = buff[i];
      if (dist[nr][nc] == INF) {
        dist[nr][nc] = dist[r][c] + 1;
        ways[nr][nc] = ways[r][c];
        parent[nr][nc] = {r,c};
        q.push({nr,nc});
      } else if (dist[nr][nc] == dist[r][c] + 1) {
        // another shortest path to (nr,nc)
        ways[nr][nc] = std::min<long long>(ways[nr][nc] + ways[r][c], 1000000LL); // cap to avoid blowups
      }
    }
  }
  int D = dist[GR][GC];
  if (D == INF) {
    return 0;
  }

  std::vector<std::pair<int,int>> path;
  {
    int r = GR, c = GC;
    path.reserve(D+1);
    while (r!=-1) {
      path.push_back({r,c});
      auto p = parent[r][c];
      r = p.first; c = p.second;
    }
    std::reverse(path.begin(), path.end());
  }

  //  turns on the shortest path (more turns ⇒ generally harder)
  auto count_turns = [&](){
    if ((int)path.size() <= 2) return 0;
    int turns = 0;
    for (int i=1;i+1<(int)path.size();++i){
      int r0=path[i-1].first, c0=path[i-1].second;
      int r1=path[i].first,   c1=path[i].second;
      int r2=path[i+1].first, c2=path[i+1].second;
      int dr1 = r1-r0, dc1 = c1-c0;
      int dr2 = r2-r1, dc2 = c2-c1;
      if (dr1!=dr2 || dc1!=dc2) ++turns;
    }
    return turns;
  };

  int T = count_turns();

  // For each path cell, count open neighbors that are NOT the prev/next path cell.
  int side_exits = 0;
  {
    std::vector<std::vector<char>> on_path(ROW, std::vector<char>(COL, 0));
    for (auto &p : path) on_path[p.first][p.second] = 1;

    for (int i=0;i<(int)path.size();++i){
      int r = path[i].first, c = path[i].second;
      int prev_r = (i>0) ? path[i-1].first : -1;
      int prev_c = (i>0) ? path[i-1].second: -1;
      int next_r = (i+1<(int)path.size()) ? path[i+1].first : -1;
      int next_c = (i+1<(int)path.size()) ? path[i+1].second: -1;

      std::array<std::pair<int,int>,4> ns;
      int cnt = neighbors(r,c,ns);
      for(int k=0;k<cnt;k++){
        int nr = ns[k].first, nc = ns[k].second;
        if (!((nr==prev_r && nc==prev_c) || (nr==next_r && nc==next_c))) {
          ++side_exits; // a branch or loop entry leaving the solution corridor
        }
      }
    }
  }

  // overall dead-ends (degree == 1), excluding S and G
  int dead_ends = 0;
  for (int r=0;r<ROW;++r){
    for (int c=0;c<COL;++c){
      if ((r==SR && c==SC) || (r==GR && c==GC)) continue;
      if (degree_rc(r,c) == 1) ++dead_ends;
    }
  }

  // number of distinct shortest paths (ways to (GR,GC) from BFS)
  long long shortest_paths = ways[GR][GC]; // >=1 if reachable

  //“single corridor” check: along the path, are there zero side exits?
  bool single_corridor = (side_exits == 0);

  // Normalize some terms roughly:
  const double Dnorm = (double)D / (double)(ROW*COL);                 // 0..~1
  const double Tnorm = (double)T / (double)std::max(1, D);            // turns per step
  const double BranchNorm = (double)side_exits / (double)std::max(1, (int)path.size());
  const double DeadNorm = (double)dead_ends / (double)std::max(1, ROW*COL);

  // Reward: long path, many turns, many side exits (decoys), many dead ends.
  // Penalize: having only 1 shortest path (no alternative routes) and single-corridor paths.
  // (You said: “if only one path leads to finish it should be easier” → we penalize it.)
  double score =
      1.20 * Dnorm +
      0.90 * Tnorm +
      1.10 * BranchNorm +
      0.60 * DeadNorm
      - 0.50 * (shortest_paths <= 1 ? 1.0 : 0.0)
      - 0.80 * (single_corridor ? 1.0 : 0.0);

  int scaled = (int)std::round(score * 1000.0);
  if (scaled < 0) scaled = 0;
  assert(ROW==COL);
  const double N2 = 1.0 * ROW * ROW;
  const double score_upper_excl = 4.9 - 4.6 / N2 - 0.9 / (N2 - 1.0); // strictly greater-than ceiling
  const int upper_bound = static_cast<int>(std::ceil(score_upper_excl * 1000.0) - 1.0);
  // const double mid = (upper_bound) / 2.0;
  // return 1.0 * abs(scaled - mid) / mid ;
  return 1.0 * scaled / upper_bound;
  // return 1.0 * (upper_bound - scaled) / upper_bound;
}

double Maze::fitness_dfs() {
  if (ROW == 0 || COL == 0) return 1.0;
  if (get_min_distance() == -1) return 1.0; // tidak solvable

  auto in_bounds = [&](int r, int c){ return r >= 0 && r < ROW && c >= 0 && c < COL; };

  auto neighbors = [&](int r, int c){
    std::vector<std::pair<int,int>> n;
    short cell = grid[r][c];
    if (!has_top_wall(cell)   && in_bounds(r-1, c)) n.push_back({r-1, c});
    if (!has_right_wall(cell) && in_bounds(r, c+1)) n.push_back({r, c+1});
    if (!has_bot_wall(cell)   && in_bounds(r+1, c)) n.push_back({r+1, c});
    if (!has_left_wall(cell)  && in_bounds(r, c-1)) n.push_back({r, c-1});
    return n;
  };

  // RNG (lebih baik daripada rand())
  std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

  auto simulate = [&](int max_steps = 10000) {
    std::stack<std::pair<int,int>> path;
    std::set<std::pair<int,int>> visited;

    std::pair<int,int> start = {0,0};
    std::pair<int,int> goal = {ROW-1, COL-1};
    path.push(start);
    visited.insert(start);

    int steps = 0;
    while (!path.empty() && steps < max_steps) {
      auto [r, c] = path.top();
      if (r == goal.first && c == goal.second) break;

      auto n = neighbors(r, c);
      std::vector<std::pair<int,int>> unvisited;
      for (auto &p : n) if (!visited.count(p)) unvisited.push_back(p);

      if (!unvisited.empty()) {
        std::uniform_int_distribution<int> dist(0, (int)unvisited.size() - 1);
        auto next = unvisited[dist(rng)];
        visited.insert(next);
        path.push(next);
      } else {
        path.pop(); // buntu, backtrack
      }
      steps++;
    }

    // Jika tidak sampai tujuan, berikan penalti besar
    if (path.empty()) return (double)max_steps;
    return (double)steps;
  };

  // Jalankan beberapa kali agar hasil stabil
  const int TRIALS = 8;
  double total_steps = 0;
  for (int i = 0; i < TRIALS; ++i) {
    total_steps += simulate();
  }
  double cost_value = total_steps / TRIALS;
  avg_steps_taken = cost_value;

  // Hitung fitness berbasis target kesulitan menengah
  double lower_bound = (double)(ROW + COL - 1);            // minimal mungkin (jalur lurus)
  double upper_bound = (double)(ROW * COL * 1.8);             // perkiraan atas: banyak backtrack
  double mid = (lower_bound + upper_bound) / 2;

  double fitness = std::fabs(cost_value - mid) / mid;
  if (fitness > 1.0) fitness = 1.0;

  return fitness;
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
