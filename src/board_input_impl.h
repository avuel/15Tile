#ifndef BOARD_INPUT_IMPL_H
#define BOARD_INPUT_IMPL_H

#ifdef BOARD_INPUT_IMPL
#pragma message("Error: Board Input already implemented (tried to include two implementations)")
#error
#else
#define BOARD_INPUT_IMPL
#endif

// Pass the interface along
#include "board_input.h"

#endif
