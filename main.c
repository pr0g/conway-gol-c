#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <curses.h>

size_t try_wrap(const int64_t index, const int64_t dim)
{
    return (index + dim) % dim;
}

size_t alive_neighbors(
    const size_t row, const size_t col,
    const size_t rows, const size_t cols,
    const bool board[rows][cols])
{
    typedef struct adjust_t
    {
        int64_t c;
        int64_t r;
    } adjust_t;

    const adjust_t adjusts[] =
    {
        { .c = -1, .r = 0 }, { .c = -1, .r = -1 },
        { .c = 0, .r = -1 }, { .c = 1, .r = -1 },
        { .c = 1, .r = 0 }, { .c = 1, .r = 1 },
        { .c = 0, .r = 1 }, { .c = -1, .r = 1 }
    };

    size_t living = 0;
    for (size_t i = 0; i < 8; i++)
    {
        living += board
            [try_wrap((int64_t)row + adjusts[i].r, (int64_t)rows)]
            [try_wrap((int64_t)col + adjusts[i].c, (int64_t)cols)];
    }

    return living;
}

bool rule_1(
    const size_t row, const size_t col,
    const size_t rows, const size_t cols,
    const bool board[rows][cols])
{
    return board[row][col] && alive_neighbors(row, col, rows, cols, board) < 2;
}

bool rule_2(
    const size_t row, const size_t col,
    const size_t rows, const size_t cols,
    const bool board[rows][cols])
{
    const size_t neighbors = alive_neighbors(row, col, rows, cols, board);
    return board[row][col] && (neighbors == 2 || neighbors == 3);
}

bool rule_3(
    const size_t row, const size_t col,
    const size_t rows, const size_t cols,
    const bool board[rows][cols])
{
    return board[row][col] && alive_neighbors(row, col, rows, cols, board) > 3;
}

bool rule_4(
    const size_t row, const size_t col,
    const size_t rows, const size_t cols,
    const bool board[rows][cols])
{
    return !board[row][col] && alive_neighbors(row, col, rows, cols, board) == 3;
}

void update_board(
    const size_t rows, const size_t cols,
    bool in_out_board[rows][cols])
{
    bool original_board[rows][cols];
    memcpy(original_board, in_out_board, sizeof(bool) * rows * cols);

    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
        {
            if (rule_1(row, col, rows, cols, original_board) ||
                rule_3(row, col, rows, cols, original_board))
            {
                in_out_board[row][col] = false;
            }
            else if (rule_2(row, col, rows, cols, original_board) ||
                    rule_4(row, col, rows, cols, original_board))
            {
                in_out_board[row][col] = true;
            }
        }
    }
}

void print_board(
    const size_t rows, const size_t cols,
    const bool board[rows][cols])
{
    const char alive_dead_display[] = { '*', '@' };
    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
        {
            printw("%c", alive_dead_display[board[row][col]]);
        }

        printw("\n");
    }
}

int main(int argc, char** argv)
{
    (void)argc; (void)argv;

    const size_t cols = 40;
    const size_t rows = 20;

    bool board[rows][cols];
    memset(board, '*', rows * cols);

    // small exploder
    // board[7][10] = true;
    // board[8][9] = true;
    // board[8][10] = true;
    // board[8][11] = true;
    // board[9][9] = true;
    // board[9][11] = true;
    // board[10][10] = true;

    // exploder
    board[8][17] = true;
    board[9][17] = true;
    board[10][17] = true;
    board[11][17] = true;
    board[12][17] = true;
    board[8][21] = true;
    board[9][21] = true;
    board[10][21] = true;
    board[11][21] = true;
    board[12][21] = true;
    board[8][19] = true;
    board[12][19] = true;

    initscr();

    for (;;) {
        print_board(rows, cols, board);
        refresh();
        update_board(rows, cols, board);
        // getchar(); // for debugging
        usleep(200000);
        clear();
    }

    endwin();

    return 0;
}
