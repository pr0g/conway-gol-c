#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

static const char on = 'a';
static const char off = '*';

size_t safe_op(const int64_t index, const int64_t dim)
{
    return (index + dim) % dim;
}

bool alive(
    const size_t row, const size_t col,
    const size_t rows, const size_t cols,
    const char board[rows][cols])
{
    return board[row][col] == on;
}

size_t alive_neighbors(
    const size_t row, const size_t col,
    const size_t rows, const size_t cols,
    const char board[rows][cols])
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
        living += alive(
            safe_op((int64_t)row + adjusts[i].r, (int64_t)rows),
            safe_op((int64_t)col + adjusts[i].c, (int64_t)cols),
            rows, cols, board);
    }

    return living;
}

bool rule_1(
    const size_t row, const size_t col,
    const size_t rows, const size_t cols,
    const char board[rows][cols])
{
    return alive(row, col, rows, cols, board) && alive_neighbors(row, col, rows, cols, board) < 2;
}

bool rule_2(
    const size_t row, const size_t col,
    const size_t rows, const size_t cols,
    const char board[rows][cols])
{
    const size_t neighbors = alive_neighbors(row, col, rows, cols, board);
    return  alive(row, col, rows, cols, board)
        &&  (neighbors == 2 || neighbors == 3);
}

bool rule_3(
    const size_t row, const size_t col,
    const size_t rows, const size_t cols,
    const char board[rows][cols])
{
    return  alive(row, col, rows, cols, board)
        &&  alive_neighbors(row, col, rows, cols, board) > 3;
}

bool rule_4(
    const size_t row, const size_t col,
    const size_t rows, const size_t cols,
    const char board[rows][cols])
{
    return  !alive(row, col, rows, cols, board) 
        &&  alive_neighbors(row, col, rows, cols, board) == 3;
}

void update_board(
    const size_t rows, const size_t cols,
    char in_out_board[rows][cols])
{
    char original_board[rows][cols];
    memcpy(original_board, in_out_board, sizeof(char) * rows * cols);

    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
        {
            if (rule_1(row, col, rows, cols, original_board) ||
                rule_3(row, col, rows, cols, original_board))
            {
                in_out_board[row][col] = off;
            }
            else if (rule_2(row, col, rows, cols, original_board) ||
                    rule_4(row, col, rows, cols, original_board))
            {
                in_out_board[row][col] = on;
            }
        }
    }
}

void print_board(
    const size_t rows, const size_t cols,
    const char board[rows][cols])
{
    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
        {
            printf("%c", board[row][col]);
        }

        printf("\n");
    }
}

int main(int argc, char** argv)
{
    (void)argc; (void)argv;

    const size_t cols = 40;
    const size_t rows = 20;

    char board[rows][cols];
    memset(board, '*', rows * cols);

    // small exploder
    board[7][10] = on;
    board[8][9] = on;
    board[8][10] = on;
    board[8][11] = on;
    board[9][9] = on;
    board[9][11] = on;
    board[10][10] = on;

    // exploder
    // board[10][20] = on;
    // board[11][20] = on;
    // board[12][20] = on;
    // board[13][20] = on;
    // board[14][20] = on;
    // board[10][24] = on;
    // board[11][24] = on;
    // board[12][24] = on;
    // board[13][24] = on;
    // board[14][24] = on;
    // board[10][22] = on;
    // board[14][22] = on;

    for (;;) {
        print_board(rows, cols, board);
        update_board(rows, cols, board);
        // getchar(); // for debugging
        usleep(200000);
        system("clear");
    }

    return 0;
}
