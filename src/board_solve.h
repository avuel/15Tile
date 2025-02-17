#ifndef BOARD_SOLVE_H
#define BOARD_SOLVE_H

#include <stdbool.h>
#include <stdint.h>

uint8_t board_solve_manhattan(uint64_t board);
bool board_solve(uint64_t board);

#endif
