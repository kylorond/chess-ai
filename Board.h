#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "Defs.h"

class Board {
public:
    // 'P', 'N', 'B', 'R', 'Q', 'K' untuk Putih
    // 'p', 'n', 'b', 'r', 'q', 'k' untuk Hitam
    // '.' untuk kotak kosong
    char board[8][8];
    bool is_white_turn;

    Board();
    void print_board();
    void make_move(const Move& move);
    void undo_move(const Move& move, char captured_piece);
    std::vector<Move> generate_legal_moves();

private:
    void generate_pawn_moves(int r, int c, std::vector<Move>& moves);
    void generate_rook_moves(int r, int c, std::vector<Move>& moves);
    void generate_knight_moves(int r, int c, std::vector<Move>& moves);
    void generate_bishop_moves(int r, int c, std::vector<Move>& moves);
    void generate_queen_moves(int r, int c, std::vector<Move>& moves);
    void generate_king_moves(int r, int c, std::vector<Move>& moves);
    bool is_valid_square(int r, int c);
    bool is_enemy_piece(int r, int c);
};

#endif // BOARD_H