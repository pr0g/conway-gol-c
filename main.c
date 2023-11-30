#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <dyn-array.h>

size_t elem_rc(const size_t row, const size_t col, const size_t cols) {
  return row * cols + col;
}

size_t try_wrap(const int64_t index, const int64_t dim) {
  return (index + dim) % dim;
}

size_t alive_neighbors(
  const size_t row, const size_t col, const size_t rows, const size_t cols,
  const bool board[]) {
  typedef struct adjust_t {
    int64_t r;
    int64_t c;
  } adjust_t;
  const adjust_t adjusts[] = {{.r = 0, .c = -1}, {.r = -1, .c = -1},
                              {.r = -1, .c = 0}, {.r = -1, .c = 1},
                              {.r = 0, .c = 1},  {.r = 1, .c = 1},
                              {.r = 1, .c = 0},  {.r = 1, .c = -1}};
  size_t living = 0;
  for (size_t i = 0; i < 8; i++) {
    living += board[elem_rc(
      try_wrap((int64_t)row + adjusts[i].r, (int64_t)rows),
      try_wrap((int64_t)col + adjusts[i].c, (int64_t)cols), cols)];
  }
  return living;
}

bool rule_1(
  const size_t row, const size_t col, const size_t rows, const size_t cols,
  const bool board[]) {
  return board[elem_rc(row, col, cols)]
      && alive_neighbors(row, col, rows, cols, board) < 2;
}

bool rule_2(
  const size_t row, const size_t col, const size_t rows, const size_t cols,
  const bool board[]) {
  const size_t neighbors = alive_neighbors(row, col, rows, cols, board);
  return board[elem_rc(row, col, cols)] && (neighbors == 2 || neighbors == 3);
}

bool rule_3(
  const size_t row, const size_t col, const size_t rows, const size_t cols,
  const bool board[]) {
  return board[elem_rc(row, col, cols)]
      && alive_neighbors(row, col, rows, cols, board) > 3;
}

bool rule_4(
  const size_t row, const size_t col, const size_t rows, const size_t cols,
  const bool board[]) {
  return !board[elem_rc(row, col, cols)]
      && alive_neighbors(row, col, rows, cols, board) == 3;
}

void update_board(const size_t rows, const size_t cols, bool board[]) {
  typedef struct change_t {
    int offset_;
    bool state_;
  } change_t;
  change_t* changes = NULL;
  for (size_t row = 0; row < rows; row++) {
    for (size_t col = 0; col < cols; col++) {
      if (
        rule_1(row, col, rows, cols, board)
        || rule_3(row, col, rows, cols, board)) {
        array_push(
          changes,
          ((change_t){.offset_ = elem_rc(row, col, cols), .state_ = false}));
      } else if (
        rule_2(row, col, rows, cols, board)
        || rule_4(row, col, rows, cols, board)) {
        array_push(
          changes,
          ((change_t){.offset_ = elem_rc(row, col, cols), .state_ = true}));
      }
    }
  }
  for (int i = 0, size = array_size(changes); i < size; i++) {
    board[changes[i].offset_] = changes[i].state_;
  }
  array_free(changes);
}

void print_board(const size_t rows, const size_t cols, const bool board[]) {
  const char alive_dead_display[] = {'*', '@'};
  for (size_t row = 0; row < rows; row++) {
    for (size_t col = 0; col < cols; col++) {
      printf("%c", alive_dead_display[board[elem_rc(row, col, cols)]]);
    }
    printf("\n");
  }
}

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  enum { cols = 40 };
  enum { rows = 27 };

  bool board[rows * cols];
  memset(board, 0, rows * cols);

  // gosper glider gun
  board[elem_rc(5, 2, cols)] = true;
  board[elem_rc(6, 2, cols)] = true;
  board[elem_rc(5, 3, cols)] = true;
  board[elem_rc(6, 3, cols)] = true;

  board[elem_rc(5, 12, cols)] = true;
  board[elem_rc(6, 12, cols)] = true;
  board[elem_rc(7, 12, cols)] = true;
  board[elem_rc(4, 13, cols)] = true;
  board[elem_rc(8, 13, cols)] = true;
  board[elem_rc(3, 14, cols)] = true;
  board[elem_rc(9, 14, cols)] = true;
  board[elem_rc(3, 15, cols)] = true;
  board[elem_rc(9, 15, cols)] = true;
  board[elem_rc(6, 16, cols)] = true;
  board[elem_rc(4, 17, cols)] = true;
  board[elem_rc(8, 17, cols)] = true;
  board[elem_rc(5, 18, cols)] = true;
  board[elem_rc(6, 18, cols)] = true;
  board[elem_rc(7, 18, cols)] = true;
  board[elem_rc(6, 19, cols)] = true;

  board[elem_rc(3, 22, cols)] = true;
  board[elem_rc(4, 22, cols)] = true;
  board[elem_rc(5, 22, cols)] = true;
  board[elem_rc(3, 23, cols)] = true;
  board[elem_rc(4, 23, cols)] = true;
  board[elem_rc(5, 23, cols)] = true;
  board[elem_rc(2, 24, cols)] = true;
  board[elem_rc(6, 24, cols)] = true;
  board[elem_rc(1, 26, cols)] = true;
  board[elem_rc(2, 26, cols)] = true;
  board[elem_rc(6, 26, cols)] = true;
  board[elem_rc(7, 26, cols)] = true;

  board[elem_rc(3, 36, cols)] = true;
  board[elem_rc(4, 36, cols)] = true;
  board[elem_rc(3, 37, cols)] = true;
  board[elem_rc(4, 37, cols)] = true;

  // eater
  board[elem_rc(20, 27, cols)] = true;
  board[elem_rc(21, 27, cols)] = true;
  board[elem_rc(20, 28, cols)] = true;
  board[elem_rc(21, 28, cols)] = true;

  board[elem_rc(21, 32, cols)] = true;
  board[elem_rc(22, 31, cols)] = true;
  board[elem_rc(22, 33, cols)] = true;
  board[elem_rc(23, 32, cols)] = true;

  board[elem_rc(23, 34, cols)] = true;
  board[elem_rc(24, 34, cols)] = true;
  board[elem_rc(25, 34, cols)] = true;
  board[elem_rc(25, 35, cols)] = true;

  // small exploder
  // board[elem_rc(7, 10, cols)] = true;
  // board[elem_rc(8, 9, cols)] = true;
  // board[elem_rc(8, 10, cols)] = true;
  // board[elem_rc(8, 11, cols)] = true;
  // board[elem_rc(9, 9, cols)] = true;
  // board[elem_rc(9, 11, cols)] = true;
  // board[elem_rc(10, 10, cols)] = true;

  // exploder
  // board[elem_rc(8, 17, cols)] = true;
  // board[elem_rc(9, 17, cols)] = true;
  // board[elem_rc(10, 17, cols)] = true;
  // board[elem_rc(11, 17, cols)] = true;
  // board[elem_rc(12, 17, cols)] = true;
  // board[elem_rc(8, 21, cols)] = true;
  // board[elem_rc(9, 21, cols)] = true;
  // board[elem_rc(10, 21, cols)] = true;
  // board[elem_rc(11, 21, cols)] = true;
  // board[elem_rc(12, 21, cols)] = true;
  // board[elem_rc(8, 19, cols)] = true;
  // board[elem_rc(12, 19, cols)] = true;

  // glider
  // board[elem_rc(10, 7, cols)] = true;
  // board[elem_rc(11, 8, cols)] = true;
  // board[elem_rc(11, 9, cols)] = true;
  // board[elem_rc(10, 9, cols)] = true;
  // board[elem_rc(9, 9, cols)] = true;

  for (;;) {
    printf("\33[H\33[2J\33[3J");
    print_board(rows, cols, board);
    update_board(rows, cols, board);
    getchar();
  }

  return 0;
}
