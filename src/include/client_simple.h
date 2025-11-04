#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <utility>

extern int rows;
extern int columns;
extern int total_mines;

char visible_map[35][35];

void Execute(int r, int c, int type);

void InitGame() {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      visible_map[i][j] = '?';
    }
  }
  
  int first_row, first_column;
  std::cin >> first_row >> first_column;
  Execute(first_row, first_column, 0);
}

void ReadMap() {
  for (int i = 0; i < rows; i++) {
    std::string line;
    std::cin >> line;
    for (int j = 0; j < columns; j++) {
      visible_map[i][j] = line[j];
    }
  }
}

void Decide() {
  // Simple strategy: just visit the first unknown cell
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      if (visible_map[i][j] == '?') {
        Execute(i, j, 0);
        return;
      }
    }
  }
}

#endif

