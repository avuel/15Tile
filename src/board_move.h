#ifndef BOARD_MOVE_H
#define BOARD_MOVE_H

#include "stdint.h"

typedef enum {
    BOARD_MOVE_NONE,
    BOARD_MOVE_LEFT,
    BOARD_MOVE_RIGHT,
    BOARD_MOVE_UP,
    BOARD_MOVE_DOWN,
    BOARD_MOVE_COUNT
} board_move;

// Interface for implementation
uint64_t board_move_left_try(uint64_t board, uint8_t blank);
uint64_t board_move_right_try(uint64_t board, uint8_t blank);
uint64_t board_move_up_try(uint64_t board, uint8_t blank);
uint64_t board_move_down_try(uint64_t board, uint8_t blank);
uint64_t board_move_try(uint64_t board, uint8_t blank, board_move move);


#define LITTLE_ENDIAN_IMPL

#ifdef LITTLE_ENDIAN_IMPL
#include "board_move_impl_little_endian.h"
#endif

#ifdef BIG_ENDIAN_IMPL
#pragma message("Error: Board Move big endian not implemented yet")
#error
#endif

#endif
