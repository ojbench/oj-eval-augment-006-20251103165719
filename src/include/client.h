#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <utility>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <cstdlib>
#include <ctime>

extern int rows;         // The count of rows of the game map.
extern int columns;      // The count of columns of the game map.
extern int total_mines;  // The count of mines of the game map.

// You MUST NOT use any other external variables except for rows, columns and total_mines.

// Global variables for client
char visible_map[35][35];  // What we can see: '?', '0'-'8', '@', 'X'
bool is_visited_client[35][35];  // Whether a cell is visited
bool is_marked_client[35][35];   // Whether a cell is marked as mine
int mine_count_client[35][35];   // Mine count for visited cells

/**
 * @brief The definition of function Execute(int, int, bool)
 *
 * @details This function is designed to take a step when player the client's (or player's) role, and the implementation
 * of it has been finished by TA. (I hope my comments in code would be easy to understand T_T) If you do not understand
 * the contents, please ask TA for help immediately!!!
 *
 * @param r The row coordinate (0-based) of the block to be visited.
 * @param c The column coordinate (0-based) of the block to be visited.
 * @param type The type of operation to a certain block.
 * If type == 0, we'll execute VisitBlock(row, column).
 * If type == 1, we'll execute MarkMine(row, column).
 * If type == 2, we'll execute AutoExplore(row, column).
 * You should not call this function with other type values.
 */
void Execute(int r, int c, int type);

/**
 * @brief The definition of function InitGame()
 *
 * @details This function is designed to initialize the game. It should be called at the beginning of the game, which
 * will read the scale of the game map and the first step taken by the server (see README).
 */
void InitGame() {
  // Initialize all global variables
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      visible_map[i][j] = '?';
      is_visited_client[i][j] = false;
      is_marked_client[i][j] = false;
      mine_count_client[i][j] = -1;
    }
  }

  int first_row, first_column;
  std::cin >> first_row >> first_column;
  Execute(first_row, first_column, 0);
}

/**
 * @brief The definition of function ReadMap()
 *
 * @details This function is designed to read the game map from stdin when playing the client's (or player's) role.
 * Since the client (or player) can only get the limited information of the game map, so if there is a 3 * 3 map as
 * above and only the block (2, 0) has been visited, the stdin would be
 *     ???
 *     12?
 *     01?
 */
void ReadMap() {
  for (int i = 0; i < rows; i++) {
    std::string line;
    std::cin >> line;
    for (int j = 0; j < columns; j++) {
      visible_map[i][j] = line[j];

      if (line[j] >= '0' && line[j] <= '8') {
        is_visited_client[i][j] = true;
        mine_count_client[i][j] = line[j] - '0';
      } else if (line[j] == '@') {
        is_marked_client[i][j] = true;
      } else if (line[j] == 'X') {
        // Hit a mine or marked wrong
        is_visited_client[i][j] = true;
      }
    }
  }
}

/**
 * @brief The definition of function Decide()
 *
 * @details This function is designed to decide the next step when playing the client's (or player's) role. Open up your
 * mind and make your decision here! Caution: you can only execute once in this function.
 */
void Decide() {
  int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};

  // Strategy 1: Try auto-explore on visited cells where all mines are marked
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      if (is_visited_client[i][j] && mine_count_client[i][j] > 0) {
        int marked_count = 0;
        int unknown_count = 0;
        for (int k = 0; k < 8; k++) {
          int ni = i + dr[k];
          int nj = j + dc[k];
          if (ni >= 0 && ni < rows && nj >= 0 && nj < columns) {
            if (is_marked_client[ni][nj]) {
              marked_count++;
            } else if (visible_map[ni][nj] == '?') {
              unknown_count++;
            }
          }
        }

        // Only auto-explore if all mines are marked AND there are unknown cells
        if (marked_count == mine_count_client[i][j] && unknown_count > 0) {
          Execute(i, j, 2);
          return;
        }
      }
    }
  }

  // Strategy 2: Mark obvious mines (all unknown neighbors are mines)
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      if (is_visited_client[i][j] && mine_count_client[i][j] >= 0) {
        int unknown_count = 0;
        int marked_count = 0;
        std::vector<std::pair<int, int>> unknown_cells;

        for (int k = 0; k < 8; k++) {
          int ni = i + dr[k];
          int nj = j + dc[k];
          if (ni >= 0 && ni < rows && nj >= 0 && nj < columns) {
            if (visible_map[ni][nj] == '?') {
              unknown_count++;
              unknown_cells.push_back({ni, nj});
            } else if (is_marked_client[ni][nj]) {
              marked_count++;
            }
          }
        }

        // If remaining unknown cells equal remaining mines, mark them all
        if (unknown_count > 0 && unknown_count + marked_count == mine_count_client[i][j]) {
          Execute(unknown_cells[0].first, unknown_cells[0].second, 1);
          return;
        }
      }
    }
  }

  // Strategy 3: Visit obvious safe cells (all mines are already marked)
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      if (is_visited_client[i][j] && mine_count_client[i][j] >= 0) {
        int marked_count = 0;
        std::vector<std::pair<int, int>> unknown_cells;

        for (int k = 0; k < 8; k++) {
          int ni = i + dr[k];
          int nj = j + dc[k];
          if (ni >= 0 && ni < rows && nj >= 0 && nj < columns) {
            if (visible_map[ni][nj] == '?') {
              unknown_cells.push_back({ni, nj});
            } else if (is_marked_client[ni][nj]) {
              marked_count++;
            }
          }
        }

        // If all mines are marked, visit unknown cells
        if (marked_count == mine_count_client[i][j] && !unknown_cells.empty()) {
          Execute(unknown_cells[0].first, unknown_cells[0].second, 0);
          return;
        }
      }
    }
  }

  // Strategy 4: Advanced pattern matching - subset/superset relationships
  // Find pairs of constraints where one is a subset of another
  for (int i1 = 0; i1 < rows; i1++) {
    for (int j1 = 0; j1 < columns; j1++) {
      if (!is_visited_client[i1][j1] || mine_count_client[i1][j1] < 0) continue;

      // Get unknown neighbors of cell 1
      std::set<std::pair<int, int>> unknown1;
      int marked1 = 0;
      for (int k = 0; k < 8; k++) {
        int ni = i1 + dr[k];
        int nj = j1 + dc[k];
        if (ni >= 0 && ni < rows && nj >= 0 && nj < columns) {
          if (visible_map[ni][nj] == '?') {
            unknown1.insert({ni, nj});
          } else if (is_marked_client[ni][nj]) {
            marked1++;
          }
        }
      }

      if (unknown1.empty()) continue;
      int remaining1 = mine_count_client[i1][j1] - marked1;

      // Compare with other constraints
      for (int i2 = 0; i2 < rows; i2++) {
        for (int j2 = 0; j2 < columns; j2++) {
          if (i1 == i2 && j1 == j2) continue;
          if (!is_visited_client[i2][j2] || mine_count_client[i2][j2] < 0) continue;

          // Get unknown neighbors of cell 2
          std::set<std::pair<int, int>> unknown2;
          int marked2 = 0;
          for (int k = 0; k < 8; k++) {
            int ni = i2 + dr[k];
            int nj = j2 + dc[k];
            if (ni >= 0 && ni < rows && nj >= 0 && nj < columns) {
              if (visible_map[ni][nj] == '?') {
                unknown2.insert({ni, nj});
              } else if (is_marked_client[ni][nj]) {
                marked2++;
              }
            }
          }

          if (unknown2.empty()) continue;
          int remaining2 = mine_count_client[i2][j2] - marked2;

          // Check if unknown1 is a subset of unknown2
          bool is_subset = true;
          for (auto cell : unknown1) {
            if (unknown2.find(cell) == unknown2.end()) {
              is_subset = false;
              break;
            }
          }

          if (is_subset && unknown1.size() < unknown2.size()) {
            // unknown1 ⊆ unknown2
            // If remaining1 == remaining2, then unknown2 - unknown1 are all safe
            if (remaining1 == remaining2) {
              for (auto [r, c] : unknown2) {
                if (unknown1.find({r, c}) == unknown1.end()) {
                  Execute(r, c, 0);
                  return;
                }
              }
            }
            // If remaining2 - remaining1 == |unknown2| - |unknown1|, then unknown2 - unknown1 are all mines
            if (remaining2 - remaining1 == (int)(unknown2.size() - unknown1.size())) {
              for (auto [r, c] : unknown2) {
                if (unknown1.find({r, c}) == unknown1.end()) {
                  Execute(r, c, 1);
                  return;
                }
              }
            }
          }
        }
      }
    }
  }

  // Strategy 5: Advanced logical deduction using constraint satisfaction
  // Build a list of all constraints (visited cells with their mine counts)
  std::vector<std::pair<int, int>> constraints;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      if (is_visited_client[i][j] && mine_count_client[i][j] >= 0) {
        // Check if this constraint has unknown neighbors
        bool has_unknown = false;
        for (int k = 0; k < 8; k++) {
          int ni = i + dr[k];
          int nj = j + dc[k];
          if (ni >= 0 && ni < rows && nj >= 0 && nj < columns && visible_map[ni][nj] == '?') {
            has_unknown = true;
            break;
          }
        }
        if (has_unknown) {
          constraints.push_back({i, j});
        }
      }
    }
  }

  // Try to find cells that can be deduced using constraint satisfaction
  std::vector<std::pair<int, int>> unknown_cells;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      if (visible_map[i][j] == '?') {
        unknown_cells.push_back({i, j});
      }
    }
  }

  // For each unknown cell, check if it can be deduced
  for (auto [r, c] : unknown_cells) {
    bool definitely_safe = false;
    bool definitely_mine = false;

    // Get all constraints that involve this cell
    std::vector<std::pair<int, int>> relevant_constraints;
    for (auto [ci, cj] : constraints) {
      for (int k = 0; k < 8; k++) {
        int ni = ci + dr[k];
        int nj = cj + dc[k];
        if (ni == r && nj == c) {
          relevant_constraints.push_back({ci, cj});
          break;
        }
      }
    }

    // Check if all constraints agree that this cell is safe
    bool all_safe = true;
    bool all_mine = true;

    for (auto [ci, cj] : relevant_constraints) {
      int marked_count = 0;
      int unknown_count = 0;
      std::vector<std::pair<int, int>> unknown_neighbors;

      for (int k = 0; k < 8; k++) {
        int ni = ci + dr[k];
        int nj = cj + dc[k];
        if (ni >= 0 && ni < rows && nj >= 0 && nj < columns) {
          if (is_marked_client[ni][nj]) {
            marked_count++;
          } else if (visible_map[ni][nj] == '?') {
            unknown_count++;
            unknown_neighbors.push_back({ni, nj});
          }
        }
      }

      int remaining_mines = mine_count_client[ci][cj] - marked_count;

      // If all remaining unknown cells must be mines
      if (remaining_mines == unknown_count && unknown_count > 0) {
        // This cell must be a mine
        if (all_mine) {
          definitely_mine = true;
        }
      } else {
        all_mine = false;
      }

      // If all mines are already marked
      if (remaining_mines == 0) {
        // This cell must be safe
        if (all_safe) {
          definitely_safe = true;
        }
      } else {
        all_safe = false;
      }
    }

    if (definitely_mine && !definitely_safe) {
      Execute(r, c, 1);
      return;
    }
    if (definitely_safe && !definitely_mine) {
      Execute(r, c, 0);
      return;
    }
  }

  // Strategy 6: Random guess on a cell with lowest mine probability
  // Prefer cells near visited cells with low mine counts
  if (!unknown_cells.empty()) {
    std::vector<std::pair<double, std::pair<int, int>>> scored_cells;

    for (auto [r, c] : unknown_cells) {
      double mine_prob = 0.0;
      int neighbor_count = 0;
      int total_info = 0;  // How much information we have about this cell

      for (int k = 0; k < 8; k++) {
        int ni = r + dr[k];
        int nj = c + dc[k];
        if (ni >= 0 && ni < rows && nj >= 0 && nj < columns &&
            is_visited_client[ni][nj] && mine_count_client[ni][nj] >= 0) {
          neighbor_count++;
          total_info++;

          int marked = 0;
          int unknown = 0;
          for (int m = 0; m < 8; m++) {
            int nni = ni + dr[m];
            int nnj = nj + dc[m];
            if (nni >= 0 && nni < rows && nnj >= 0 && nnj < columns) {
              if (is_marked_client[nni][nnj]) {
                marked++;
              } else if (visible_map[nni][nnj] == '?') {
                unknown++;
              }
            }
          }

          if (unknown > 0) {
            mine_prob += (double)(mine_count_client[ni][nj] - marked) / unknown;
          }
        }
      }

      if (neighbor_count > 0) {
        mine_prob /= neighbor_count;
      } else {
        // Unknown area
        mine_prob = (double)total_mines / (rows * columns);
      }

      // Prefer cells with more information (more visited neighbors)
      // and lower mine probability
      // Use a combined score: mine_prob - 0.01 * total_info
      // This slightly prefers cells with more information
      double score = mine_prob - 0.01 * total_info;

      scored_cells.push_back({score, {r, c}});
    }

    std::sort(scored_cells.begin(), scored_cells.end());
    Execute(scored_cells[0].second.first, scored_cells[0].second.second, 0);
    return;
  }

  // If we reach here, there are no unknown cells left
  // This should not happen in a normal game, but just in case
  // The game should have already ended
}

#endif