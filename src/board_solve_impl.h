#ifndef BOARD_SOLVE_IMPL_H
#define BOARD_SOLVE_IMPL_H

#ifdef BOARD_SOLVE_IMPL
#pragma message("Error: Board Solve already implemented (tried to include two implementations)")
#error
#else
#define BOARD_SOLVE_IMPL
#endif

// Pass the interface along
#include "board_solve.h"

#endif
