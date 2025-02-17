#include "board_render_impl.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

    
static const char* tiles_decimal[16] = {

    " 0",
    " 1",
    " 2",
    " 3",
    " 4",
    " 5",
    " 6",
    " 7",
    " 8",
    " 9",
    "10",
    "11",
    "12",
    "13",
    "14",
    "15",

};

static const char* tiles_hexadecimal[16] = {

    " 0",
    " 1",
    " 2",
    " 3",
    " 4",
    " 5",
    " 6",
    " 7",
    " 8",
    " 9",
    " a",
    " b",
    " c",
    " d",
    " e",
    "  "
    //" f"

};

static char*  board_text        = NULL;
static char*  board_clear       = NULL;
static size_t board_row_gaps    = (size_t)0;
static size_t board_column_gaps = (size_t)0;
static size_t board_width       = (size_t)0;
static size_t board_height      = (size_t)0;
static size_t board_size        = (size_t)0;

static char* board_digit_offsets[4] = { 0 };


/* *------------------------------------------------* */
/* |                                                | */
/* |                                                | */
/* |                                                | */
/* |                                                | */
/* |        00        01        02        03        | */
/* |                                                | */
/* |                                                | */
/* |                                                | */
/* |                                                | */
/* |        00        01        02        03        | */
/* |                                                | */
/* |                                                | */
/* |                                                | */
/* |                                                | */
/* |        00        01        02        03        | */
/* |                                                | */
/* |                                                | */
/* |                                                | */
/* |                                                | */
/* |        00        01        02        03        | */
/* |                                                | */
/* |                                                | */
/* |                                                | */
/* |                                                | */
/* *------------------------------------------------* */

void board_render_resize(uint16_t row_gaps, uint16_t column_gaps)
{
    board_row_gaps = (size_t)row_gaps;
    uint16_t count_horizontal_borders = 2;
    uint16_t count_horizontal_spaces  = column_gaps * 5;
    uint16_t count_digits             = 8;
    board_width = (size_t)count_horizontal_borders + (size_t)count_horizontal_spaces + (size_t)count_digits + 1;

    board_column_gaps = (size_t)column_gaps;
    uint16_t count_vertical_borders = 2;
    uint16_t count_vertical_spaces  = row_gaps * 5;
    uint16_t count_digit_rows       = 4;
    board_height = (size_t)count_vertical_borders + (size_t)count_vertical_spaces + (size_t)count_digit_rows;

    board_size = board_width * board_height + 1; /* + 1 for \0 */

    free(board_text);
    free(board_clear);

    board_text = malloc(board_size * sizeof(char));
    assert(board_text != NULL);

    size_t count_border_vertical_dashes = (size_t)board_width - 3;
    size_t count_border_vertical_spaces = count_border_vertical_dashes;

    board_text[0] = '*';
    board_text[board_width - 2] = '*';
    board_text[board_width - 1] = '\n';

    size_t offset = 1;
    memset(board_text + offset, '-', count_border_vertical_dashes);

    offset = offset + board_width;

    const char tile_first [2] = "00";
    const char tile_second[2] = "01";
    const char tile_third [2] = "02";
    const char tile_fourth[2] = "03";

    char* dest = NULL;

    for (uint8_t n = 0; n < 4; ++n)
    {
        for (uint8_t i = 0; i < (row_gaps + 1); ++i)
        {
            /* prepare the start of the row */
            board_text[offset-1] = '|';
            memset(board_text + offset, ' ', count_border_vertical_spaces);

            offset = offset + board_width;

            /* finish the end of the row */
            board_text[offset-3] = '|';
            board_text[offset-2] = '\n';
        }

        /* add 2 spaces after each copy due to size we copied*/
        size_t digit_index = (size_t)n * 4;


        dest = board_text + offset - board_width;
        board_digit_offsets[n] = dest;


        /* finish the end of the row */
        board_text[offset-3] = '|';
        board_text[offset-2] = '\n';
    }

    for (uint8_t i = 0; i < row_gaps; ++i)
    {
        /* prepare the start of the row */
        board_text[offset-1] = '|';
        memset(board_text + offset, ' ', count_border_vertical_spaces);

        offset = offset + board_width;

        /* finish the end of the row */
        board_text[offset-3] = '|';
        board_text[offset-2] = '\n';
    }

    board_text[offset - 1] = '*';
    board_text[offset + board_width - 3] = '*';

    memset(board_text + offset, '-', count_border_vertical_dashes);

    offset = offset + board_width - 1;

    //fprintf(stderr, "Offset %zu\n", offset);
    //fprintf(stderr, "Size: %zu\n", board_size);

    /* new line */
    board_text[board_size - 2] = '\n';

    /* null terminate */
    board_text[board_size - 1] = '\0';
}

static uint8_t board_digits[16] = { 0 };

void board_render_update(uint64_t board)
{
    size_t offset = 0;
    char* dest = NULL;
    uint8_t  tile_index = 0;

    uint8_t* tile_ptr = (uint8_t*)&board;
    uint8_t  tile     = NULL;

    for (uint8_t i = 0x08; i > 0; --i)
    {
        tile = *tile_ptr;
        tile_index = 2*(i-1);
        
        board_digits[tile_index + 1] = tile & 0x0F;
        board_digits[tile_index    ] = tile >> 4;

        //fprintf(stderr, "Digit %hhu: %hhu\n", tile_index    , board_digits[tile_index    ]);
        //fprintf(stderr, "Digit %hhu: %hhu\n", tile_index + 1, board_digits[tile_index + 1]);

        tile_ptr = tile_ptr + 1;
    }

    //const char tile_first [2] = "00";
    //const char tile_second[2] = "01";
    //const char tile_third [2] = "02";
    //const char tile_fourth[2] = "03";

    uint8_t digits_first  = 0;
    uint8_t digits_second = 0;
    uint8_t digits_third  = 0;
    uint8_t digits_fourth = 0;

    const char* tile_first  = NULL;
    const char* tile_second = NULL;
    const char* tile_third  = NULL;
    const char* tile_fourth = NULL;

    for (uint8_t n = 0; n < 4; ++n)
    {

        /* add 2 spaces after each copy due to size we copied*/
        //dest = board_text + offset - board_width;
        //dest = memcpy(dest + board_column_gaps, tile_first, 2);
        //dest = memcpy(dest + board_column_gaps + 2, tile_second, 2);
        //dest = memcpy(dest + board_column_gaps + 2, tile_third,  2);
        //       memcpy(dest + board_column_gaps + 2, tile_fourth, 2);
        size_t digit_index = (size_t)n * 4;

        digits_first  = board_digits[digit_index    ];
        digits_second = board_digits[digit_index + 1];
        digits_third  = board_digits[digit_index + 2];
        digits_fourth = board_digits[digit_index + 3];

        //fprintf(stderr, "Digits %zu: %hhu\n", digit_index    , digits_first);
        //fprintf(stderr, "Digits %zu: %hhu\n", digit_index + 1, digits_second);
        //fprintf(stderr, "Digits %zu: %hhu\n", digit_index + 2, digits_third);
        //fprintf(stderr, "Digits %zu: %hhu\n", digit_index + 3, digits_fourth);

        tile_first  = tiles_hexadecimal[digits_first];
        tile_second = tiles_hexadecimal[digits_second];
        tile_third  = tiles_hexadecimal[digits_third];
        tile_fourth = tiles_hexadecimal[digits_fourth];

        //fprintf(stderr, "Tile %zu: %c%c\n", digit_index    , tile_first[0] , tile_first[1]);
        //fprintf(stderr, "Tile %zu: %c%c\n", digit_index + 1, tile_second[0],tile_second[1]);
        //fprintf(stderr, "Tile %zu: %c%c\n", digit_index + 2, tile_third[0] , tile_third[1]);
        //fprintf(stderr, "Tile %zu: %c%c\n", digit_index + 3, tile_fourth[0],tile_fourth[1]);

        dest = board_digit_offsets[n];
        dest = memcpy(dest + board_column_gaps    , tile_first , 2);
        dest = memcpy(dest + board_column_gaps + 2, tile_second, 2);
        dest = memcpy(dest + board_column_gaps + 2, tile_third ,  2);
               memcpy(dest + board_column_gaps + 2, tile_fourth, 2);
    }

}

void board_render_clear(FILE* stream)
{
    /* VT100 escape code to clear screen */
    fputs("\033[H\033[J", stderr);
}

void board_render_print(FILE* stream)
{
    fputs(board_text, stream);
}
