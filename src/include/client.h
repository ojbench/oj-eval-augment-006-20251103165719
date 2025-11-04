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
      if (is_visited_client[i][j] && mine_count_client[i][j] > 0) {
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

  // Strategy 4: Advanced logical deduction
  // Find cells that can be deduced as safe or mines through constraint satisfaction
  std::vector<std::pair<int, int>> unknown_cells;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      if (visible_map[i][j] == '?') {
        unknown_cells.push_back({i, j});
      }
    }
  }

  // Try to find a safe cell using constraint propagation
  for (auto [r, c] : unknown_cells) {
    bool definitely_safe = false;
    bool definitely_mine = false;

    // Check all visited neighbors
    std::vector<std::pair<int, int>> visited_neighbors;
    for (int k = 0; k < 8; k++) {
      int ni = r + dr[k];
      int nj = c + dc[k];
      if (ni >= 0 && ni < rows && nj >= 0 && nj < columns &&
          is_visited_client[ni][nj] && mine_count_client[ni][nj] >= 0) {
        visited_neighbors.push_back({ni, nj});
      }
    }

    // Check if this cell must be safe or mine based on constraints
    for (auto [vi, vj] : visited_neighbors) {
      int marked_count = 0;
      int unknown_count = 0;

      for (int k = 0; k < 8; k++) {
        int ni = vi + dr[k];
        int nj = vj + dc[k];
        if (ni >= 0 && ni < rows && nj >= 0 && nj < columns) {
          if (is_marked_client[ni][nj]) {
            marked_count++;
          } else if (visible_map[ni][nj] == '?') {
            unknown_count++;
          }
        }
      }

      // If all mines are already marked, this cell is safe
      if (marked_count == mine_count_client[vi][vj]) {
        definitely_safe = true;
        break;
      }
    }

    if (definitely_safe) {
      Execute(r, c, 0);
      return;
    }
  }

  // Strategy 5: Random guess on a cell with lowest mine probability
  // Prefer cells near visited cells with low mine counts
  if (!unknown_cells.empty()) {
    std::vector<std::pair<double, std::pair<int, int>>> scored_cells;

    for (auto [r, c] : unknown_cells) {
      double mine_prob = 0.0;
      int neighbor_count = 0;

      for (int k = 0; k < 8; k++) {
        int ni = r + dr[k];
        int nj = c + dc[k];
        if (ni >= 0 && ni < rows && nj >= 0 && nj < columns &&
            is_visited_client[ni][nj] && mine_count_client[ni][nj] >= 0) {
          neighbor_count++;

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
        mine_prob = 0.5;  // Unknown area
      }

      scored_cells.push_back({mine_prob, {r, c}});
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