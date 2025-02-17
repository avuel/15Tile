#include "board_solve_impl.h"

#include "board_move.h"
#include "board_render.h"

#include <string.h>
#include <stdio.h>

#define BOARD_SOLVE_MOVE_LIMIT 128

/* A table of pre-computed manhattan distances for each tile *
 * Each row corresponds to the 15-rowth tile so that they    *
 * the tiles can be accessed sequentially in memory since    *
 * the first value  read from the board is in the 15th tile  */
static const int8_t manhattan_table_reversed[16][16] = {
/*  Tile 15  */
{   6,5,4,3,
    5,4,3,2,
    4,3,2,1,
    3,2,1,0  },
/*  Tile 14  */
{   5,4,3,4,
    4,3,2,3,
    3,2,1,2,
    2,1,0,1  },
/*  Tile 13  */
{   4,3,4,5,
    3,2,3,4,
    2,1,2,3,
    1,0,1,2  },
/*  Tile 12  */
{   3,4,5,6,
    2,3,4,5,
    1,2,3,4,
    0,1,2,3  },
/*  Tile 11  */
{   5,4,3,2,
    4,3,2,1,
    3,2,1,0,
    4,3,2,1  },
/*  Tile 10  */
{   4,3,2,3,
    3,2,1,2,
    2,1,0,1,
    3,2,1,2  },
/*  Tile  9  */
{   3,2,3,4,
    2,1,2,3,
    1,0,1,2,
    2,1,2,3  },
/*  Tile  8  */
{   2,3,4,5,
    1,2,3,4,
    0,1,2,3,
    1,2,3,4  },
/*  Tile  7  */
{   4,3,2,1,
    3,2,1,0,
    4,3,2,1,
    5,4,3,2  },
/*  Tile  6  */
{   3,2,1,2,
    2,1,0,1,
    3,2,1,2,
    4,3,2,3  },
/*  Tile  5  */
{   2,1,2,3,
    1,0,1,2,
    2,1,2,3,
    3,2,3,4  },
/*  Tile  4  */
{   1,2,3,4,
    0,1,2,3,
    1,2,3,4,
    2,3,4,5  },
/*  Tile  3  */
{   3,2,1,0,
    4,3,2,1,
    5,4,3,2,
    6,5,4,3  },
/*  Tile  2  */
{   2,1,0,1,
    3,2,1,2,
    4,3,2,3,
    5,4,3,4  },
/*  Tile  1  */
{   1,0,1,2,
    2,1,2,3,
    3,2,3,4,
    4,3,4,5  },
/*  Tile  0  */
{   0,1,2,3,
    1,2,3,4,
    2,3,4,5,
    3,4,5,6  }};

uint8_t board_solve_manhattan(uint64_t board)
{
    /* Unrolled Loop
    *  uint8_t sum = 0;
    *  
    *  for (int8_t i = 0; i < 16; ++i)
    *  {    
    *       sum = sum + manhattan[i][board & 0x0F];
    *       board >> 4;
    *  }
    *  
    *  return sum;
    */

    uint8_t sum = 0;

    /* Tiles are reversed in the array to access them sequentially in memory */
    sum = sum + manhattan_table_reversed[ 0][board & 0x0F]; board = board >> 4;
    sum = sum + manhattan_table_reversed[ 1][board & 0x0F]; board = board >> 4;
    sum = sum + manhattan_table_reversed[ 2][board & 0x0F]; board = board >> 4;
    sum = sum + manhattan_table_reversed[ 3][board & 0x0F]; board = board >> 4;
    sum = sum + manhattan_table_reversed[ 4][board & 0x0F]; board = board >> 4;
    sum = sum + manhattan_table_reversed[ 5][board & 0x0F]; board = board >> 4;
    sum = sum + manhattan_table_reversed[ 6][board & 0x0F]; board = board >> 4;
    sum = sum + manhattan_table_reversed[ 7][board & 0x0F]; board = board >> 4;
    sum = sum + manhattan_table_reversed[ 8][board & 0x0F]; board = board >> 4;
    sum = sum + manhattan_table_reversed[ 9][board & 0x0F]; board = board >> 4;
    sum = sum + manhattan_table_reversed[10][board & 0x0F]; board = board >> 4;
    sum = sum + manhattan_table_reversed[11][board & 0x0F]; board = board >> 4;
    sum = sum + manhattan_table_reversed[12][board & 0x0F]; board = board >> 4;
    sum = sum + manhattan_table_reversed[13][board & 0x0F]; board = board >> 4;
    sum = sum + manhattan_table_reversed[14][board & 0x0F]; board = board >> 4;
    sum = sum + manhattan_table_reversed[15][board & 0x0F];

    return sum;
}

static const uint64_t solve_final = 0x0123456789ABCDEF;

static uint8_t board_solve_move_count = UINT8_MAX;
static board_move board_solve_moves_temp[BOARD_SOLVE_MOVE_LIMIT] = { 0 };
static board_move board_solve_moves[BOARD_SOLVE_MOVE_LIMIT] = { 0 };
static uint64_t board_solve_search_count = 0;

static bool board_solve_internal(uint64_t board, uint8_t blank, uint8_t h, uint8_t move_count, uint8_t max_depth)
{
    ++board_solve_search_count;

    //fprintf(stderr, "Search: %zu\nMove Count: %hhu\nh: %hhu\nMax Depth: %hhu\n\n", board_solve_search_count, move_count, h, max_depth);
    //board_render_update(board);
    //board_render_print(stderr);

    uint16_t depth = (uint16_t)move_count + (uint16_t)h;

    if (depth > (uint16_t)max_depth)
    {
        return false;
    }

    if (board == solve_final)
    {
        if (move_count < board_solve_move_count)
        {
            board_solve_move_count = move_count;
            memset(board_solve_moves, BOARD_MOVE_NONE, BOARD_SOLVE_MOVE_LIMIT);
            memcpy(board_solve_moves, board_solve_moves_temp, BOARD_SOLVE_MOVE_LIMIT);
            fprintf(stderr, "Solved! Move Count: %hhu\n", board_solve_move_count);
        }

        //board_render_update(board);
        //board_render_print(stderr);

        return true;
    }

    uint8_t solve_move_count_current = board_solve_move_count;

    uint64_t board_left  = board_move_left_try(board, blank);
    //fputs("Board Left\n", stderr);
    //board_render_update(board_left);
    //board_render_print(stderr);

    uint64_t board_right = board_move_right_try(board, blank);
    //fputs("Board Right\n", stderr);
    //board_render_update(board_right);
    //board_render_print(stderr);

    uint64_t board_up    = board_move_up_try(board, blank);
    //fputs("Board Up\n", stderr);
    //board_render_update(board_up);
    //board_render_print(stderr);

    uint64_t board_down  = board_move_down_try(board, blank);
    //fputs("Board Down\n", stderr);
    //board_render_update(board_down);
    //board_render_print(stderr);

    bool solved = false;

    if (board_left > 0)
    {
        //fputs("Solve move left\n", stderr);

        board_solve_moves_temp[move_count] = BOARD_MOVE_LEFT;

        uint8_t blank_left = blank - 1;
        uint8_t h_left = board_solve_manhattan(board_left);
        bool solved_left = board_solve_internal(board_left, blank_left, h_left, move_count + 1, max_depth);

        if (solved_left)
        {
            //board_solve_move_count = move_count;
            //board_solve_moves[move_count] = BOARD_MOVE_LEFT;

            solved = true;
        }
    }

    if (board_right > 0)
    {
        //fputs("Solve move right\n", stderr);

        board_solve_moves_temp[move_count] = BOARD_MOVE_RIGHT;

        uint8_t blank_right = blank + 1;
        uint8_t h_right = board_solve_manhattan(board_right);
        bool solved_right = board_solve_internal(board_right, blank_right, h_right, move_count + 1, max_depth);

        if (solved_right)
        {
            //board_solve_move_count = move_count;
            //board_solve_moves[move_count] = BOARD_MOVE_RIGHT;

            solved = true;
        }
    }

    if (board_up > 0)
    {
        //fputs("Solve move right\n", stderr);

        board_solve_moves_temp[move_count] = BOARD_MOVE_UP;

        uint8_t blank_up = blank - 4;
        uint8_t h_up = board_solve_manhattan(board_up);
        bool solved_up = board_solve_internal(board_up, blank_up, h_up, move_count + 1, max_depth);

        if (solved_up)
        {
            //board_solve_move_count = move_count;
            //board_solve_moves[move_count] = BOARD_MOVE_UP;

            solved = true;
        }
    }

    if (board_down > 0)
    {
        //fputs("Solve move down\n", stderr);

        board_solve_moves_temp[move_count] = BOARD_MOVE_DOWN;

        uint8_t blank_down = blank + 4;
        uint8_t h_down = board_solve_manhattan(board_down);
        bool solved_down = board_solve_internal(board_down, blank_down, h_down, move_count + 1, max_depth);

        if (solved_down)
        {
            //board_solve_move_count = move_count;
            //board_solve_moves[move_count] = BOARD_MOVE_DOWN;

            solved = true;
        }
    }

    //if (solved)
    //{
        //fprintf(stderr, "Move: %hhu\n", move_count);
        //board_render_update(board);
        //board_render_print(stderr);
    //}

    return solved;
}

static void board_solve_reset()
{
    board_solve_move_count = UINT8_MAX;
    board_solve_search_count = 0;
    memset(board_solve_moves, BOARD_MOVE_NONE, BOARD_SOLVE_MOVE_LIMIT);
    memset(board_solve_moves_temp, BOARD_MOVE_NONE, BOARD_SOLVE_MOVE_LIMIT);
}

bool board_solve(uint64_t board)
{
    fputs("Solving...\n", stderr);
    board_solve_reset();

    uint64_t board_temp = board;
    uint8_t blank = 0;
    uint8_t h = board_solve_manhattan(board);

    /* find the blank tile */
    for (uint8_t i = 0; i < 15; ++i)
    {
        if ((board_temp & 0x0F) == 15)
        {
            blank = 15 - i;
            break;
        }

        board_temp = board_temp >> 4; 
    }

    //fprintf(stderr, "Solving board: %zx\nBlank: %hhu\n", board, blank);

    bool solved = false;

    uint8_t start_depth = h % 2;
    uint8_t max_depth = BOARD_SOLVE_MOVE_LIMIT;

    for (uint8_t i = start_depth; i < max_depth; i+=2)
    {
        //fprintf(stderr, "Searching at depth: %hhu    ", i);
        solved = board_solve_internal(board, blank, h, 0, i);

        if (solved)
        {
            break;
        }

        //fprintf(stderr, "Number of boards searched: %zu\n", board_solve_search_count);
    }

    if (solved)
    {
        fprintf(stderr, "Solved: %hhu moves\n", board_solve_move_count);
        fprintf(stderr, "Boards Searched: %zu\n", board_solve_search_count);

        for (uint8_t i = 0; i < board_solve_move_count; ++i)
        {
            if (board_solve_move_count >= BOARD_SOLVE_MOVE_LIMIT)
            {
                fprintf(stderr, "Error: Too many moves in solution: %hhu\n", board_solve_move_count);
                break;
            }

            switch (board_solve_moves[i])
            {
                case BOARD_MOVE_RIGHT:
                {
                    fprintf(stderr, "Move %hhu: RIGHT\n", i + 1);
                    break;
                }

                case BOARD_MOVE_LEFT:
                {
                    fprintf(stderr, "Move %hhu: LEFT\n", i + 1);
                    break;
                }

                case BOARD_MOVE_UP:
                {
                    fprintf(stderr, "Move %hhu: UP\n", i + 1);
                    break;
                }

                case BOARD_MOVE_DOWN:
                {
                    fprintf(stderr, "Move %hhu: DOWN\n", i + 1);
                    break;
                }
                case BOARD_MOVE_NONE:
                {
                    fprintf(stderr, "Move %hhu: NONE\n", i + 1);
                    break;
                }

                default:
                {
                    fputs("Error: Invalid move in solve\n", stderr);
                    break;
                }
            }
        }
    }

    return solved;
}

static void print_manhattan_distances(void)
{
    fputs("static const int8_t manhattan[16][16] = {\n", stderr);
    uint8_t row = 0;
    uint8_t col = 0;
    uint8_t row_desired = 0;
    uint8_t col_desired = 0;

    uint8_t row_diff = 0;
    uint8_t col_diff = 0;

    uint8_t h = 0;

    for (int16_t i = 15; i > -1; --i)
    {
        fputs("{\n", stderr);

        col_desired = i % 4;
        row_desired = i / 4;

        for (int16_t j = 0; j < 16; ++j)
        {
            if (j % 4 == 0)
            {
                fputs("    ", stderr);
            }

            col = j % 4;
            row = j / 4;

            if (row > row_desired)
            {
                row_diff = row - row_desired;
            }

            else
            {
                row_diff = row_desired - row;
            }

            if (col > col_desired)
            {
                col_diff = col - col_desired;
            }

            else
            {
                col_diff = col_desired - col;
            }
            
            //fprintf(stderr, "%hhu,", row_diff + col_diff);
            fprintf(stderr, "%hhu", row_diff + col_diff);

            if (j % 4 == 3)
            {
                fputs("\n", stderr);
            }

            else
            {
                fputs(",", stderr);
            }
        }

        if (i < 15)
        {
            fputs("},\n\n", stderr);
        }

        else
        {
            fputs("}\n\n", stderr);
        }
    }
    fputs("};", stderr);
}

static void print_manhattan_distances_matrix(void)
{
    uint8_t row = 0;
    uint8_t col = 0;
    uint8_t row_desired = 0;
    uint8_t col_desired = 0;

    uint8_t row_diff = 0;
    uint8_t col_diff = 0;

    uint8_t h = 0;

    for (int16_t i = 15; i > -1; --i)
    {

        col_desired = i % 4;
        row_desired = i / 4;

        for (int16_t j = 0; j < 16; ++j)
        {
            col = j % 4;
            row = j / 4;

            if (row > row_desired)
            {
                row_diff = row - row_desired;
            }

            else
            {
                row_diff = row_desired - row;
            }

            if (col > col_desired)
            {
                col_diff = col - col_desired;
            }

            else
            {
                col_diff = col_desired - col;
            }
            
            fprintf(stderr, "%hhu ", row_diff + col_diff);
        }

        fputs("\n", stderr);
    }
}
