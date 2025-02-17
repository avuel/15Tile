#ifndef BOARD_RENDER_H
#define BOARD_RENDER_H

#include <stdint.h>
#include <stdio.h>

void board_render_resize(uint16_t row_gaps, uint16_t columns_gap);
void board_render_update(uint64_t board);
void board_render_clear(FILE* stream);
void board_render_print(FILE* stream);

#endif
