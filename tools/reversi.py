from enum import IntEnum

class Piece(IntEnum):
    NONE = 0
    WHITE = 1
    BLACK = 2

type Board = list[list[Piece]]

BOARD_WIDTH = BOARD_HEIGHT = 8

def is_in_range(x: int, y: int) -> bool:
    """Return if the coordinates exist on the board space"""

    return 0 <= x < BOARD_WIDTH and 0 <= y < BOARD_HEIGHT

def is_valid_move(board: Board, piece: Piece, x: int, y: int) -> bool:
    """Return if the piece is able to make a valid move on the board at the position"""

    if piece == Piece.NONE or not is_in_range(x, y) or board[y][x] != Piece.NONE:
        return False

    directions = [(1, 0), (1, 1), (0, 1), (-1, 1), (-1, 0), (-1, -1), (0, -1), (1, -1)]

    for dx, dy in directions:
        i, j = x + dx, y + dy

        if is_in_range(i, j) and board[j][i] in (Piece.NONE, piece):
            continue

        while is_in_range(i := i + dx, j := j + dy) and board[j][i] not in (Piece.NONE, piece):
            pass

        if is_in_range(i, j) and board[j][i] == piece:
            return True

    return False

def flip(board: Board, piece: Piece, x: int, y: int, dx: int, dy: int) -> None:
    """Flip the line at x, y until it reaches the same piece, but not at x, y"""

    while is_in_range(x := x + dx, y := y + dy) and board[y][x] != piece:
        board[y][x] = piece

def place(board: Board, piece: Piece, x: int, y: int) -> None:
    """Place the piece at the given space and flip the corresponding pieces"""

    if piece == Piece.NONE or not is_in_range(x, y) or board[y][x] != Piece.NONE:
        return

    directions = [(1, 0), (1, 1), (0, 1), (-1, 1), (-1, 0), (-1, -1), (0, -1), (1, -1)]

    for dx, dy in directions:
        i, j = x + dx, y + dy

        if is_in_range(i, j) and board[j][i] in (Piece.NONE, piece):
            continue

        while is_in_range(i := i + dx, j := j + dy) and board[j][i] not in (Piece.NONE, piece):
            pass

        if is_in_range(i, j) and board[j][i] == piece:
            flip(board, piece, x, y, dx, dy)

    board[y][x] = piece

PIECES_TO_ASCII = {Piece.NONE: ".", Piece.WHITE: "W", Piece.BLACK: "K"}

def display_board(board: Board) -> None:
    """Print the board to the console"""

    for y in range(BOARD_HEIGHT):
        for x in range(BOARD_WIDTH):
            print(PIECES_TO_ASCII[board[y][x]], end="")
        print()

def display_moves(board: Board, piece: Piece) -> None:
    """Print the board to the console with the available moves"""

    for y in range(BOARD_HEIGHT):
        for x in range(BOARD_WIDTH):
            print("o" if is_valid_move(board, piece, x, y) else PIECES_TO_ASCII[board[y][x]], end="")
        print()
