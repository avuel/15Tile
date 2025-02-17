#ifndef BOARD_MOVE_IMPL_LITTLE_ENDIAN_H
#define BOARD_MOVE_IMPL_LITTLE_ENDIAN_H

#ifdef BOARD_MOVE_IMPL
#pragma message("Error: Board Move already implemented (tried to include two implementations)")
#error
#else
#define BOARD_MOVE_IMPL
#endif

// Pass the interface along
#include "board_move.h"

#endif
