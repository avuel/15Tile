#include "board_move_impl_little_endian.h"

#include <stdio.h>

/* (uint8)(tile >> 1) = shift index */
static const uint8_t shifts[8] = {
    7, 6, 5, 4, 3, 2, 1, 0
};

static uint64_t move_left(uint64_t board, uint8_t blank)
{

    uint8_t shift = shifts[blank >> 1];

    //fprintf(stderr, "Blank + 1 = %hhu\n", blank + 1);
    uint8_t offset = (blank + 1) % 2;
    //fprintf(stderr, "Shift: %hhx\n", shift);
    //fprintf(stderr, "Offset: %hhx\n", offset);

    uint8_t* right_ptr = ((uint8_t*)&board) + shift;
    uint8_t* left_ptr  = right_ptr + offset;
    uint8_t  right = *right_ptr;
    uint8_t  left  = *left_ptr;

    //fprintf(stderr, "Board Value: %hhx\n", *(uint8_t*)(&blank));
    //fprintf(stderr, "Right Location: %p\n", (void*)(right_ptr));
    //fprintf(stderr, "Left Location: %p\n", (void*)(left_ptr));

    if (offset != 0)
    {
        //fprintf(stderr, "Left Before: %hhx\n", left);
        //fprintf(stderr, "Right Before: %hhx\n", right);

        (*right_ptr) = (right & 0x0F) | (left * 16);

        if (left != right)
        {
            (*left_ptr) = (left & 0xF0) | (right >> 4);
        }

        //fprintf(stderr, "Left After: %hhx\n", *left_ptr);
        //fprintf(stderr, "Right After: %hhx\n", *right_ptr);
    }

    else
    {
        //fprintf(stderr, "Right Value: %hhx\n", right_value);
        (*right_ptr) = (right >> 4) | (right * 16);
    }

    return board;
}

static uint64_t move_up(uint64_t board, uint8_t blank)
{
    //fprintf(stderr, "Blank: %hhu\n", blank);
    uint8_t shift = shifts[blank >> 1];

    uint8_t* bot_ptr = ((uint8_t*)&board) + shift;
    uint8_t* top_ptr = bot_ptr + 2;
    uint8_t  bot = *(bot_ptr);
    uint8_t  top = *(top_ptr);

    //fprintf(stderr, "Bot: %hhx\n", bot);
    //fprintf(stderr, "Top: %hhx\n", top);

    uint8_t offset   = ((blank + 1) % 2) << 2;
    uint8_t bits     = 0x000F << offset;
    uint8_t bits_not = 0x00FF & ~bits;

    //fprintf(stderr, "Bits: %hhx\n", bits);

    (*bot_ptr) = (bot & bits_not) | (top & bits);
    (*top_ptr) = (top & bits_not) | (bot & bits);

    return board;
}

uint64_t board_move_left_try(uint64_t board, uint8_t blank)
{
    blank = blank & 0x000F;

    if (blank % 4  == (uint8_t)0)
    {
        return (uint64_t)0;
    }

    return move_left(board, blank);
}

uint64_t board_move_right_try(uint64_t board, uint8_t blank)
{
    blank = blank & 0x000F;

    if (blank % 4  == (uint8_t)3)
    {
        return (uint64_t)0;
    }

    /* Moving blank right is equivalent to moving tile to the right left */
    return move_left(board, blank + 1);
}

uint64_t board_move_up_try(uint64_t board, uint8_t blank)
{
    blank = blank & 0x000F;

    if ((blank >> 2)  == (uint8_t)0)
    {
        return (uint64_t)0;
    }

    return move_up(board, blank);
}

uint64_t board_move_down_try(uint64_t board, uint8_t blank)
{
    blank = blank & 0x000F;

    if ((blank >> 2)  == (uint8_t)3)
    {
        return (uint64_t)0;
    }

    /* Moving blank down is equivalent to moving tile below up */
    return move_up(board, blank + 4);
}

uint64_t board_move_try(uint64_t board, uint8_t blank, board_move move)
{
    if (move == BOARD_MOVE_LEFT)
    {
        return board_move_left_try(board, blank);
    }

    if (move == BOARD_MOVE_RIGHT)
    {
        return board_move_right_try(board, blank);
    }

    if (move == BOARD_MOVE_UP)
    {
        return board_move_up_try(board, blank);
    }

    if (move == BOARD_MOVE_DOWN)
    {
        return board_move_down_try(board, blank);
    }
}
