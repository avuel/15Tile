#include <stdarg.h> 
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "board_input.h"
#include "board_move.h"
#include "board_render.h"
#include "board_solve.h"

int main(void)
{
    //print_manhattan_distances();
    /* HEURISTIC TESTING */
    /*
    uint64_t sums[100] = { 0 };
    //uint64_t board = 0x0123456789ABCDEF;
    //int8_t blank = 15;

    clock_t start = clock(), diff = { 0 };

    for (uint64_t i = 0; i < 100000000; ++i)
    {
        uint64_t h = manhattan(i);

        if (0 <= h && h < 100)
        {
            sums[h] = sums[h] + 1;
        }
    }
    diff = clock() - start;
    int msec = diff * 1000 / CLOCKS_PER_SEC;
    
    for (uint64_t i = 0; i < 100; ++i)
    {
        fprintf(stderr,"%zu: %zu\n", i, sums[i]);
    }

    fprintf(stderr, "\nTime taken %d seconds %d milliseconds\n", msec/1000, msec%1000);
    //print_manhattan_distances();
    */

    /* MOVE TESTING */
    /*
    uint64_t board = 0x0123456789ABCDEF;
    uint64_t board_new = board;

    uint8_t blank = 0x000F;
    
    print_board(board);

    for (uint8_t i = (uint8_t)0x0000; i < (uint8_t)0x0010; ++i)
    {
        const char* move = tiles_decimal[i];

        //fprintf(stderr, "Board Pre:  %zx\n", board);
        board_new = board_move_left_try(board, blank);
        //fprintf(stderr, "Board Post: %zx\n", board_new);
        
        if (board_new != 0)
        {
            board = board_new;
            fprintf(stderr, "*------------------- Move  %s -------------------*\n\n", move);
            print_board(board);
        }

        --blank;
    }
    */

    /* RENDER TESTING */
    /*
    uint64_t board = 0x0123456789ABCDEF;
    uint8_t  blank = 0x000F;

    uint8_t row_gaps    = 4;
    uint8_t column_gaps = 8;

    board_render_resize(row_gaps, column_gaps);
    board_render_update(board);
    board_render_clear(stderr);
    board_render_print(stderr);

    uint64_t new_board = board;

    for (uint8_t i = 0; i < 15; ++i)
    {
        //fprintf(stderr, "%hhx\n", i);
        new_board = board_move_left_try(new_board, blank);

        if (new_board > 0)
        {
            board = new_board;
            board_render_update(board);
            board_render_clear(stderr);
            board_render_print(stderr);
        }

        else
        {
            new_board = board;
        }

        --blank;
    }
    */

    /* INPUT TESTING */
    //board_input_initialize();
    //board_input_start();
    
    uint64_t board = 0x0123456789ABCDEF;
    uint8_t  blank = 15;

    uint8_t row_gaps    = 4;
    uint8_t column_gaps = 8;

    board_render_resize(row_gaps, column_gaps);

    board_render_update(board);
    board_render_clear(stderr);
    board_render_print(stderr);
    
    uint64_t new_board = board;
    uint8_t  new_blank = blank;

    clock_t start = clock();
    clock_t diff = { 0 };
    float sec = 0.0f;

    bool running = true;
    int key_code = 0;

    while (running)
    {
        board_input_poll();

        /* Terminate Execution */
        if (board_input_key_just_pressed(BOARD_KEY_QUIT) || board_input_key_just_pressed(BOARD_KEY_ESCAPE))
        {
            running = false;
            continue;
        }

        new_board = 0;

        if (board_input_key_just_pressed(BOARD_KEY_P))
        {
            bool solved = board_solve(board);
        }

        else if (board_input_key_just_pressed(BOARD_KEY_W))
        {
            new_board = board_move_up_try(board, blank);
            new_blank = blank - 4;
        }

        else if (board_input_key_just_pressed(BOARD_KEY_A))
        {
            new_board = board_move_left_try(board, blank);
            new_blank = blank - 1;
        }

        else if (board_input_key_just_pressed(BOARD_KEY_S))
        {
            new_board = board_move_down_try(board, blank);
            new_blank = blank + 4;
        }

        else if (board_input_key_just_pressed(BOARD_KEY_D))
        {
            new_board = board_move_right_try(board, blank);
            new_blank = blank + 1;
        }

        if (new_board > 0)
        {
            board = new_board;
            blank = new_blank;
            board_render_update(board);
            board_render_clear(stderr);
            board_render_print(stderr);
            /* ^[[1A VT100 key code for moving up one line, also go back to start of lines */
            /* fputs("\033[A\r", stderr); */
            uint8_t h = board_solve_manhattan(board);
            //fprintf(stderr, "h: %hhu   \n", h);
        }

        diff = clock() - start;
        sec = (float)diff / CLOCKS_PER_SEC;

        Sleep(10);
    }

    return EXIT_SUCCESS;
}
