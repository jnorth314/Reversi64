#ifndef _REVERSI_H_
#define _REVERSI_H_

#include <stdbool.h>

#define BOARD_WIDTH  (8)
#define BOARD_HEIGHT (8)

#if (BOARD_WIDTH%2 != 0 || BOARD_HEIGHT%2 != 0)
#error "Board Dimensions Must Be Divisible By 2!"
#endif

#if (BOARD_WIDTH <= 2 || BOARD_HEIGHT <= 2)
#error "Board Dimensions Must Be Greater Than 2!"
#endif

typedef enum {
    PIECE_NONE,
    PIECE_WHITE,
    PIECE_BLACK,
    NUM_PIECES
} Piece;

typedef struct {
    Piece tiles[BOARD_HEIGHT][BOARD_WIDTH];
} Board;

bool is_valid_move(Board* board, Piece piece, int x, int y);
bool has_valid_move(Board* board, Piece piece);
Piece next_player(Board* board, Piece player);
void flip(Board* board, Piece piece, int x, int y, int dx, int dy);
void place(Board* board, Piece piece, int x, int y);
void reset(Board* board);

void display_board(void);
void display_pieces(Board* board);
void display_moves(Board* board, Piece player);
void display_cursor(int x, int y);

#endif /* _REVERSI_H_ */
