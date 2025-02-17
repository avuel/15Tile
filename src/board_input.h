#ifndef BOARD_INPUT_H
#define BOARD_INPUT_H

#include <stdbool.h>
#include <stdint.h>

#include "board_input_keycodes.h"

//void board_input_initialize(void);
//void board_input_start(void);
//void board_input_stop(void);
//void board_input_destroy(void);

void board_input_poll(void);
void board_input_reset(void);
bool board_input_key_just_pressed(uint8_t key_code);


#endif
