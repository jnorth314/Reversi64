#include "reversi.h"

#include <libdragon.h>

static Board board;
static int x, y;
static Piece player;

void start(void) {
    reset(&board);
    x = y = 0;
    player = PIECE_WHITE;
}

void set_next_player(void) {
    Piece other = player == PIECE_WHITE ? PIECE_BLACK : PIECE_WHITE;

    if (has_valid_move(&board, other))
        player = other;
    else if (!has_valid_move(&board, player))
        player = PIECE_NONE;
}

inline int clamp(int n, int min, int max) {
    if (n < min) return min;
    if (n > max) return max;
    return n;
}

void update(void) {
    joypad_buttons_t buttons = joypad_get_buttons_pressed(JOYPAD_PORT_1);

    if (buttons.d_left)
        x--;
    if (buttons.d_right)
        x++;
    if (buttons.d_up)
        y--;
    if (buttons.d_down)
        y++;

    x = clamp(x, 0, BOARD_WIDTH - 1);
    y = clamp(y, 0, BOARD_HEIGHT - 1);

    if (buttons.a && is_valid_move(&board, player, x, y)) {
        place(&board, player, x, y);
        set_next_player();
    }

    if (buttons.start)
        start();
}

void render(void) {
    display_board();
    display_pieces(&board);
    display_moves(&board, player);
    display_cursor(x, y);
}

int main() {
    joypad_init();
    rdpq_init();

    display_init(RESOLUTION_640x480, DEPTH_16_BPP, 2, GAMMA_NONE, FILTERS_DISABLED);

    rdpq_font_t* font = rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_MONO);
    rdpq_text_register_font(1, font);

    start();

    while (1) {
        rdpq_attach_clear(display_get(), NULL);
        joypad_poll();

        update();
        render();

        rdpq_detach_show();
    }
}
