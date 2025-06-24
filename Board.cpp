#include "Board.h"
#include <iostream>

Board::Board() {
    // Inisialisasi papan catur ke posisi awal
    char initial_board[8][8] = {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
    };
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            board[i][j] = initial_board[i][j];
        }
    }
    is_white_turn = true;
}

void Board::print_board() {
    std::cout << "\n   a b c d e f g h\n";
    std::cout << "  -----------------\n";
    for (int i = 0; i < 8; ++i) {
        std::cout << 8 - i << "| ";
        for (int j = 0; j < 8; ++j) {
            std::cout << board[i][j] << " ";
        }
        std::cout << "|" << 8 - i << "\n";
    }
    std::cout << "  -----------------\n";
    std::cout << "   a b c d e f g h\n";
    std::cout << (is_white_turn ? "Giliran Putih" : "Giliran Hitam") << std::endl;
}

void Board::make_move(const Move& move) {
    board[move.to_row][move.to_col] = board[move.from_row][move.from_col];
    board[move.from_row][move.from_col] = '.';
    is_white_turn = !is_white_turn; // Ganti giliran
}

void Board::undo_move(const Move& move, char captured_piece) {
    board[move.from_row][move.from_col] = board[move.to_row][move.to_col];
    board[move.to_row][move.to_col] = captured_piece;
    is_white_turn = !is_white_turn; // Kembalikan giliran
}

std::vector<Move> Board::generate_legal_moves() {
    std::vector<Move> moves;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            char piece = board[r][c];
            if (piece == '.') continue;

            // Cek apakah buah catur ini milik pemain yang sedang giliran
            bool piece_is_white = isupper(piece);
            if (piece_is_white != is_white_turn) continue;
            
            char piece_type = tolower(piece);
            switch (piece_type) {
                case 'p': generate_pawn_moves(r, c, moves); break;
                case 'r': generate_rook_moves(r, c, moves); break;
                case 'n': generate_knight_moves(r, c, moves); break;
                case 'b': generate_bishop_moves(r, c, moves); break;
                case 'q': generate_queen_moves(r, c, moves); break;
                case 'k': generate_king_moves(r, c, moves); break;
            }
        }
    }
    return moves;
}

bool Board::is_valid_square(int r, int c) {
    return r >= 0 && r < 8 && c >= 0 && c < 8;
}

bool Board::is_enemy_piece(int r, int c) {
    if (!is_valid_square(r, c) || board[r][c] == '.') return false;
    return isupper(board[r][c]) != is_white_turn;
}

// Implementasi generator gerakan (versi sederhana)
// NOTE: Tidak menangani en passant, promosi, atau castling untuk simplisitas

void Board::generate_pawn_moves(int r, int c, std::vector<Move>& moves) {
    int direction = is_white_turn ? -1 : 1;
    int start_row = is_white_turn ? 6 : 1;

    // Maju satu kotak
    if (is_valid_square(r + direction, c) && board[r + direction][c] == '.') {
        moves.push_back({r, c, r + direction, c});
        // Maju dua kotak dari posisi awal
        if (r == start_row && board[r + 2 * direction][c] == '.') {
            moves.push_back({r, c, r + 2 * direction, c});
        }
    }
    // Menangkap secara diagonal
    if (is_valid_square(r + direction, c - 1) && is_enemy_piece(r + direction, c - 1)) {
        moves.push_back({r, c, r + direction, c - 1});
    }
    if (is_valid_square(r + direction, c + 1) && is_enemy_piece(r + direction, c + 1)) {
        moves.push_back({r, c, r + direction, c + 1});
    }
}

void Board::generate_knight_moves(int r, int c, std::vector<Move>& moves) {
    int dr[] = {-2, -2, -1, -1, 1, 1, 2, 2};
    int dc[] = {-1, 1, -2, 2, -2, 2, -1, 1};
    for(int i = 0; i < 8; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        if (is_valid_square(nr, nc) && (board[nr][nc] == '.' || is_enemy_piece(nr, nc))) {
            moves.push_back({r, c, nr, nc});
        }
    }
}

void Board::generate_rook_moves(int r, int c, std::vector<Move>& moves) {
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    for(int i = 0; i < 4; ++i) {
        for(int j = 1; j < 8; ++j) {
            int nr = r + dr[i] * j;
            int nc = c + dc[i] * j;
            if (!is_valid_square(nr, nc)) break;
            if (board[nr][nc] == '.') {
                moves.push_back({r, c, nr, nc});
            } else {
                if (is_enemy_piece(nr, nc)) {
                    moves.push_back({r, c, nr, nc});
                }
                break;
            }
        }
    }
}

void Board::generate_bishop_moves(int r, int c, std::vector<Move>& moves) {
    int dr[] = {-1, -1, 1, 1};
    int dc[] = {-1, 1, -1, 1};
    for(int i = 0; i < 4; ++i) {
        for(int j = 1; j < 8; ++j) {
            int nr = r + dr[i] * j;
            int nc = c + dc[i] * j;
            if (!is_valid_square(nr, nc)) break;
            if (board[nr][nc] == '.') {
                moves.push_back({r, c, nr, nc});
            } else {
                if (is_enemy_piece(nr, nc)) {
                    moves.push_back({r, c, nr, nc});
                }
                break;
            }
        }
    }
}

void Board::generate_queen_moves(int r, int c, std::vector<Move>& moves) {
    generate_rook_moves(r, c, moves);
    generate_bishop_moves(r, c, moves);
}

void Board::generate_king_moves(int r, int c, std::vector<Move>& moves) {
    int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    for(int i = 0; i < 8; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        if (is_valid_square(nr, nc) && (board[nr][nc] == '.' || is_enemy_piece(nr, nc))) {
            moves.push_back({r, c, nr, nc});
        }
    }
}