#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

size_t offset = 0;

const char on = 'a';
const char off = '*';

size_t safe_op(int64_t index, int64_t dim)
{
    return (index + dim) % dim;
}

bool alive(size_t row, size_t col, size_t rows, size_t cols, char board[rows][cols])
{
    return board[row][col] == on;
}

size_t alive_neighbors(size_t row, size_t col, size_t rows, size_t cols, char board[rows][cols])
{
    typedef struct adjust_t
    {
        int64_t c;
        int64_t r;
    } adjust_t;

    adjust_t adjusts[] = { { .c = -1, .r = 0 }, { .c = -1, .r = -1 }, { .c = 0, .r = -1 }, { .c = 1, .r = -1 },
                           { .c = 1, .r = 0 }, { .c = 1, .r = 1 }, { .c = 0, .r = 1 }, { .c = -1, .r = 1 }};

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

bool rule_1(size_t row, size_t col, size_t rows, size_t cols, char board[rows][cols])
{
    return alive(row, col, rows, cols, board) && alive_neighbors(row, col, rows, cols, board) < 2;
}

bool rule_2(size_t row, size_t col, size_t rows, size_t cols, char board[rows][cols])
{
    size_t neighbors = alive_neighbors(row, col, rows, cols, board);
    return alive(row, col, rows, cols, board) && (neighbors == 2 || neighbors == 3);
}

bool rule_3(size_t row, size_t col, size_t rows, size_t cols, char board[rows][cols])
{
    return alive(row, col, rows, cols, board) && alive_neighbors(row, col, rows, cols, board) > 3;
}

bool rule_4(size_t row, size_t col, size_t rows, size_t cols, char board[rows][cols])
{
    return !alive(row, col, rows, cols, board) && alive_neighbors(row, col, rows, cols, board) == 3;
}

void print_board(size_t rows, size_t cols, char board[rows][cols])
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

void update_board(size_t rows, size_t cols, char board[rows][cols])
{
    char original_board[rows][cols];
    memcpy(original_board, board, sizeof(char) * rows * cols);

    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
        {
            if (rule_1(row, col, rows, cols, original_board) ||
                rule_3(row, col, rows, cols, original_board))
            {
                board[row][col] = off;
            }
            else if (rule_2(row, col, rows, cols, original_board) ||
                    rule_4(row, col, rows, cols, original_board))
            {
                board[row][col] = on;
            }
            
            // if (rule_1(row, col, rows, cols, board))
            // {
            //     board[row][col] = '*';
            // }
        }
    }

    // board[0][offset++] = 'a';
    // if (offset == rows * cols)
    // {
    //     offset = 0;
    // }

    // print_board(rows, cols, original_board);
}

int main(int argc, char** argv)
{
    const size_t cols = 40;
    const size_t rows = 20;

    char board[rows][cols];
    memset(board, '*', rows * cols);

    //print_board(rows, cols, board);

    board[7][10] = on;
    board[8][9] = on;
    board[8][10] = on;
    board[8][11] = on;
    board[9][9] = on;
    board[9][11] = on;
    board[10][10] = on;

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

    // board[1][0] = on;
    // board[1][2] = on;

    //size_t alive = alive_neighbors(3, 3, rows, cols, board);

    //print_board(rows, cols, board);

    //printf("alive: %zu\n", alive);

    for (;;) {
        print_board(rows, cols, board);
        update_board(rows, cols, board);
        // getchar();
        usleep(200000);
        system("clear");
    }

    return 0;
}