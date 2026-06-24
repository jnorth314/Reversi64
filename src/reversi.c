#include "reversi.h"

#include <libdragon.h>

inline bool is_in_range(int x, int y) {
    return x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT;
}

bool is_valid_move(Board* board, Piece piece, int x, int y) {
    if (piece == PIECE_NONE || !is_in_range(x, y) || board->tiles[y][x] != PIECE_NONE)
        return false;

    static const int directions[8][2] = {{1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};

    for (int i = 0; i < 8; i++) {
        int dx = directions[i][0];
        int dy = directions[i][1];

        int j = x + dx;
        int k = y + dy;

        if (is_in_range(j, k) && (board->tiles[k][j] == PIECE_NONE || board->tiles[k][j] == piece))
            continue;

        do {
            j += dx;
            k += dy;
        } while (is_in_range(j, k) && (board->tiles[k][j] != PIECE_NONE && board->tiles[k][j] != piece));

        if (is_in_range(j, k) && board->tiles[k][j] == piece)
            return true;
    }

    return false;
}

bool has_valid_move(Board* board, Piece piece) {
    for (int y = 0; y < BOARD_HEIGHT; y++)
        for (int x = 0; x < BOARD_WIDTH; x++)
            if (is_valid_move(board, piece, x, y))
                return true;

    return false;
}

void flip(Board* board, Piece piece, int x, int y, int dx, int dy) {
    do {
        board->tiles[y][x] = piece;
        x += dx;
        y += dy;
    } while (is_in_range(x, y) && board->tiles[y][x] != piece);
}

void place(Board* board, Piece piece, int x, int y) {
    if (piece == PIECE_NONE || !is_in_range(x, y) || board->tiles[y][x] != PIECE_NONE)
        return;

    static const int directions[8][2] = {{1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};

    for (int i = 0; i < 8; i++) {
        int dx = directions[i][0];
        int dy = directions[i][1];

        int j = x + dx;
        int k = y + dy;

        if (is_in_range(j, k) && (board->tiles[k][j] == PIECE_NONE || board->tiles[k][j] == piece))
            continue;

        do {
            j += dx;
            k += dy;
        } while (is_in_range(j, k) && (board->tiles[k][j] != PIECE_NONE && board->tiles[k][j] != piece));

        if (is_in_range(j, k) && board->tiles[k][j] == piece)
            flip(board, piece, x, y, dx, dy);
    }

    board->tiles[y][x] = piece;
}

void reset(Board* board) {
    sys_hw_memset32(&board->tiles, PIECE_NONE, sizeof(board->tiles));
    board->tiles[3][3] = board->tiles[4][4] = PIECE_WHITE;
    board->tiles[3][4] = board->tiles[4][3] = PIECE_BLACK;
}

static const int TILE_WIDTH = 48;
static const int TILE_HEIGHT = 48;
static const int PIECE_RADIUS = 16;
static const int MOVE_RADIUS = 4;
static const float BOARD_X0 = (640 - TILE_WIDTH*BOARD_WIDTH) / 2;
static const float BOARD_Y0 = (480 - TILE_HEIGHT*BOARD_HEIGHT) / 2;

static const color_t PIECE_COLORS[NUM_PIECES] = {
    RGBA32(0, 0, 0, 0),         //PIECE_NONE
    RGBA32(255, 255, 255, 255), //PIECE_WHITE
    RGBA32(0, 0, 0, 255)        //PIECE_BLACK
};

inline void circle(int x, int y, int r) {
    static const int NUM_TRIANGLES = 8;
    static const float PI = 3.1415926535f;

    float v1[] = { x, y };

    float vertices[NUM_TRIANGLES + 1][2] = {};

    for (int i = 0; i < NUM_TRIANGLES + 1; i++){
        vertices[i][0] = x + r*fm_cosf(2*PI*i/NUM_TRIANGLES);
        vertices[i][1] = y + r*fm_sinf(2*PI*i/NUM_TRIANGLES);
    }

    for (int i = 0; i < NUM_TRIANGLES; i++)
        rdpq_triangle(&TRIFMT_FILL, v1, vertices[i], vertices[i + 1]);
}

void display_board(void) {
    // Board
    rdpq_set_mode_fill(RGBA32(0, 255, 0, 255));
    rdpq_fill_rectangle(BOARD_X0, BOARD_Y0, BOARD_X0 + TILE_WIDTH*BOARD_WIDTH, BOARD_Y0 + TILE_HEIGHT*BOARD_HEIGHT);

    // Grid
    rdpq_set_mode_fill(RGBA32(64, 64, 64, 255));
    for (int x = 1; x < BOARD_WIDTH; x++)
        rdpq_fill_rectangle(BOARD_X0 + TILE_WIDTH*x, BOARD_Y0, BOARD_X0 + TILE_WIDTH*x + 1, BOARD_Y0 + TILE_HEIGHT*BOARD_HEIGHT);
    for (int y = 1; y < BOARD_HEIGHT; y++)
        rdpq_fill_rectangle(BOARD_X0, BOARD_Y0 + TILE_HEIGHT*y, BOARD_X0 + TILE_WIDTH*BOARD_WIDTH, BOARD_Y0 + TILE_HEIGHT*y + 1);
}

void display_pieces(Board* board) {
    rdpq_set_mode_standard();
    rdpq_mode_combiner(RDPQ_COMBINER_FLAT);

    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            Piece piece = board->tiles[y][x];

            if (piece == PIECE_NONE)
                continue;

            rdpq_set_prim_color(PIECE_COLORS[piece]);
            circle(BOARD_X0 + TILE_WIDTH*x + TILE_WIDTH/2, BOARD_Y0 + TILE_HEIGHT*y + TILE_HEIGHT/2, PIECE_RADIUS);
        }
    }
}

void display_moves(Board* board, Piece player) {
    rdpq_set_mode_standard();
    rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
    rdpq_set_prim_color(PIECE_COLORS[player]);

    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (is_valid_move(board, player, x, y))
                circle(BOARD_X0 + TILE_WIDTH*x + TILE_WIDTH/2, BOARD_Y0 + TILE_HEIGHT*y + TILE_HEIGHT/2, MOVE_RADIUS);
        }
    }

}

void display_cursor(int x, int y) {
    static const int PLAYER_RADIUS = 4;

    rdpq_set_mode_standard();
    rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
    rdpq_set_prim_color(RGBA32(255, 0, 0, 255));
    circle(BOARD_X0 + TILE_WIDTH*x + TILE_WIDTH/2, BOARD_Y0 + TILE_HEIGHT*y + TILE_HEIGHT/2, PLAYER_RADIUS);
}
