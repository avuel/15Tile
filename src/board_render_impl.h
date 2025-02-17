#ifndef BOARD_RENDER_IMPL_H
#define BOARD_RENDER_IMPL_H

#ifdef BOARD_RENDER_IMPL
#pragma message("Error: Board Render already implemented (tried to include two implementations)")
#error
#else
#define BOARD_RENDER_IMPL
#endif

// Pass the interface along
#include "board_render.h"

#endif
