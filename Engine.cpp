#include "Engine.h"
#include <limits>
#include <map>

int Engine::evaluate(Board& board) {
    int score = 0;
    std::map<char, int> piece_values = {
        {'P', 100}, {'N', 320}, {'B', 330}, {'R', 500}, {'Q', 900}, {'K', 20000},
        {'p', -100}, {'n', -320}, {'b', -330}, {'r', -500}, {'q', -900}, {'k', -20000}
    };

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            char piece = board.board[r][c];
            if (piece != '.') {
                score += piece_values[piece];
            }
        }
    }
    return score;
}

Move Engine::find_best_move(Board& board, int depth) {
    Move best_move;
    int best_value = board.is_white_turn ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
    
    auto legal_moves = board.generate_legal_moves();
    
    for (const auto& move : legal_moves) {
        char captured_piece = board.board[move.to_row][move.to_col];
        board.make_move(move);
        
        int board_value = minimax(board, depth - 1, !board.is_white_turn);
        
        board.undo_move(move, captured_piece);
        
        if (board.is_white_turn) {
            if (board_value > best_value) {
                best_value = board_value;
                best_move = move;
            }
        } else {
            if (board_value < best_value) {
                best_value = board_value;
                best_move = move;
            }
        }
    }
    return best_move;
}

int Engine::minimax(Board& board, int depth, bool maximizing_player) {
    if (depth == 0) {
        return evaluate(board);
    }
    
    auto legal_moves = board.generate_legal_moves();
    if (legal_moves.empty()) {
        // Ini bisa berarti skakmat atau stalemate. Untuk simplisitas, kita anggap evaluasi biasa.
        // Mesin yang lebih canggih akan memeriksa status skakmat.
        return evaluate(board);
    }

    if (maximizing_player) { // Giliran Putih (mencari skor maksimal)
        int max_eval = std::numeric_limits<int>::min();
        for (const auto& move : legal_moves) {
            char captured_piece = board.board[move.to_row][move.to_col];
            board.make_move(move);
            int eval = minimax(board, depth - 1, false);
            board.undo_move(move, captured_piece);
            max_eval = std::max(max_eval, eval);
        }
        return max_eval;
    } else { // Giliran Hitam (mencari skor minimal)
        int min_eval = std::numeric_limits<int>::max();
        for (const auto& move : legal_moves) {
            char captured_piece = board.board[move.to_row][move.to_col];
            board.make_move(move);
            int eval = minimax(board, depth - 1, true);
            board.undo_move(move, captured_piece);
            min_eval = std::min(min_eval, eval);
        }
        return min_eval;
    }
}