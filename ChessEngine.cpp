#include "ChessEngine.h"
#include <iostream>
#include <limits>
#include <stdexcept>

// Notasi FEN (Forsyth-Edwards Notation) untuk posisi awal
const std::string FEN_START_POS = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

ChessEngine::ChessEngine() {
    // Inisialisasi papan dari FEN
    board.assign(8, std::vector<char>(8, ' '));
    int r = 0, c = 0;
    for (char fen_char : FEN_START_POS) {
        if (fen_char == '/') {
            r++;
            c = 0;
        } else if (isdigit(fen_char)) {
            c += fen_char - '0';
        } else {
            board[r][c] = fen_char;
            c++;
        }
    }

    // Nilai materi untuk evaluasi
    pieceValues = {
        {'P', 10}, {'N', 30}, {'B', 30}, {'R', 50}, {'Q', 90}, {'K', 900},
        {'p', -10}, {'n', -30}, {'b', -30}, {'r', -50}, {'q', -90}, {'k', -900}
    };
}

void ChessEngine::printBoard() {
    std::cout << "\n  a b c d e f g h\n";
    std::cout << " +-----------------+\n";
    for (int i = 0; i < 8; ++i) {
        std::cout << 8 - i << "| ";
        for (int j = 0; j < 8; ++j) {
            std::cout << board[i][j] << " ";
        }
        std::cout << "|" << 8 - i << std::endl;
    }
    std::cout << " +-----------------+\n";
    std::cout << "  a b c d e f g h\n" << std::endl;
}

Move ChessEngine::parseMove(const std::string& moveStr) {
    if (moveStr.length() != 4) {
        throw std::invalid_argument("Format langkah salah. Gunakan format 'e2e4'.");
    }
    int start_col = moveStr[0] - 'a';
    int start_row = 7 - (moveStr[1] - '1');
    int end_col = moveStr[2] - 'a';
    int end_row = 7 - (moveStr[3] - '1');
    
    if (start_row < 0 || start_row > 7 || start_col < 0 || start_col > 7 ||
        end_row < 0 || end_row > 7 || end_col < 0 || end_col > 7) {
        throw std::invalid_argument("Koordinat di luar papan.");
    }
    
    return {start_row, start_col, end_row, end_col, board[start_row][start_col]};
}

bool ChessEngine::isMoveValid(const Move& move, bool isWhiteTurn) {
    char piece = board[move.start_row][move.start_col];
    if (piece == ' ') return false;
    bool isWhitePiece = isupper(piece);
    if ((isWhiteTurn && !isWhitePiece) || (!isWhiteTurn && isWhitePiece)) {
        return false; // Bukan giliran pemain
    }
    
    // Validasi sederhana: Cek apakah langkah ada di daftar langkah yang sah
    std::vector<Move> validMoves = generateMoves(isWhiteTurn);
    for(const auto& validMove : validMoves) {
        if (validMove.start_row == move.start_row && validMove.start_col == move.start_col &&
            validMove.end_row == move.end_row && validMove.end_col == move.end_col) {
            return true;
        }
    }
    return false;
}


void ChessEngine::makeMove(const Move& move) {
    board[move.end_row][move.end_col] = move.piece;
    board[move.start_row][move.start_col] = ' ';
}

void ChessEngine::undoMove(const Move& move, char capturedPiece) {
    board[move.start_row][move.start_col] = move.piece;
    board[move.end_row][move.end_col] = capturedPiece;
}

int ChessEngine::evaluateBoard() {
    int score = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (pieceValues.count(board[i][j])) {
                score += pieceValues[board[i][j]];
            }
        }
    }
    return score;
}

Move ChessEngine::getBestMove(int depth, bool isMaximizingPlayer) {
    Move bestMove;
    int bestValue = isMaximizingPlayer ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
    
    std::vector<Move> possibleMoves = generateMoves(isMaximizingPlayer);

    for (const auto& move : possibleMoves) {
        char capturedPiece = board[move.end_row][move.end_col];
        makeMove(move);
        
        int boardValue = minimax(depth - 1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), !isMaximizingPlayer);
        
        undoMove(move, capturedPiece);

        if (isMaximizingPlayer) {
            if (boardValue > bestValue) {
                bestValue = boardValue;
                bestMove = move;
            }
        } else {
            if (boardValue < bestValue) {
                bestValue = boardValue;
                bestMove = move;
            }
        }
    }
    return bestMove;
}

int ChessEngine::minimax(int depth, int alpha, int beta, bool isMaximizingPlayer) {
    if (depth == 0) {
        return evaluateBoard();
    }

    std::vector<Move> possibleMoves = generateMoves(isMaximizingPlayer);
    
    if (possibleMoves.empty()) { // Checkmate atau stalemate
        return evaluateBoard();
    }

    if (isMaximizingPlayer) {
        int maxEval = std::numeric_limits<int>::min();
        for (const auto& move : possibleMoves) {
            char capturedPiece = board[move.end_row][move.end_col];
            makeMove(move);
            int eval = minimax(depth - 1, alpha, beta, false);
            undoMove(move, capturedPiece);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                break; // Beta cutoff
            }
        }
        return maxEval;
    } else {
        int minEval = std::numeric_limits<int>::max();
        for (const auto& move : possibleMoves) {
            char capturedPiece = board[move.end_row][move.end_col];
            makeMove(move);
            int eval = minimax(depth - 1, alpha, beta, true);
            undoMove(move, capturedPiece);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                break; // Alpha cutoff
            }
        }
        return minEval;
    }
}

std::vector<Move> ChessEngine::generateMoves(bool isMaximizingPlayer) {
    std::vector<Move> moves;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            char piece = board[r][c];
            if (piece == ' ') continue;

            bool isWhitePiece = isupper(piece);
            if ((isMaximizingPlayer && !isWhitePiece) || (!isMaximizingPlayer && isWhitePiece)) {
                continue;
            }

            switch (tolower(piece)) {
                case 'p': getPawnMoves(moves, r, c, isWhitePiece); break;
                case 'r': getRookMoves(moves, r, c); break;
                case 'n': getKnightMoves(moves, r, c); break;
                case 'b': getBishopMoves(moves, r, c); break;
                case 'q': getQueenMoves(moves, r, c); break;
                case 'k': getKingMoves(moves, r, c); break;
            }
        }
    }
    return moves;
}

// Implementasi fungsi-fungsi get<Piece>Moves
// Ini adalah implementasi yang disederhanakan
// NOTE: Kode di bawah ini cukup panjang, ini adalah inti dari "aturan" catur

void ChessEngine::getPawnMoves(std::vector<Move>& moves, int r, int c, bool isWhite) {
    char piece = board[r][c];
    int direction = isWhite ? -1 : 1;
    int startRow = isWhite ? 6 : 1;

    // Gerak maju satu langkah
    if (r + direction >= 0 && r + direction < 8 && board[r + direction][c] == ' ') {
        moves.push_back({r, c, r + direction, c, piece});
        // Gerak maju dua langkah dari posisi awal
        if (r == startRow && board[r + 2 * direction][c] == ' ') {
            moves.push_back({r, c, r + 2 * direction, c, piece});
        }
    }

    // Makan diagonal
    for (int dc = -1; dc <= 1; dc += 2) {
        if (c + dc >= 0 && c + dc < 8 && r + direction >= 0 && r + direction < 8) {
            char target = board[r + direction][c + dc];
            if (target != ' ' && (isupper(target) != isWhite)) {
                moves.push_back({r, c, r + direction, c + dc, piece});
            }
        }
    }
}

void ChessEngine::getRookMoves(std::vector<Move>& moves, int r, int c) {
    char piece = board[r][c];
    bool isWhite = isupper(piece);
    int dirs[] = {-1, 1, 0, 0};
    int dcs[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; ++i) {
        for (int j = 1; j < 8; ++j) {
            int nr = r + dirs[i] * j;
            int nc = c + dcs[i] * j;
            if (nr < 0 || nr >= 8 || nc < 0 || nc >= 8) break;
            char target = board[nr][nc];
            if (target == ' ') {
                moves.push_back({r, c, nr, nc, piece});
            } else {
                if (isupper(target) != isWhite) {
                    moves.push_back({r, c, nr, nc, piece});
                }
                break;
            }
        }
    }
}

void ChessEngine::getKnightMoves(std::vector<Move>& moves, int r, int c) {
    char piece = board[r][c];
    bool isWhite = isupper(piece);
    int dr[] = {-2, -2, -1, -1, 1, 1, 2, 2};
    int dc[] = {-1, 1, -2, 2, -2, 2, -1, 1};

    for (int i = 0; i < 8; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
            char target = board[nr][nc];
            if (target == ' ' || (isupper(target) != isWhite)) {
                moves.push_back({r, c, nr, nc, piece});
            }
        }
    }
}

void ChessEngine::getBishopMoves(std::vector<Move>& moves, int r, int c) {
    char piece = board[r][c];
    bool isWhite = isupper(piece);
    int dirs[] = {-1, -1, 1, 1};
    int dcs[] = {-1, 1, -1, 1};

    for (int i = 0; i < 4; ++i) {
        for (int j = 1; j < 8; ++j) {
            int nr = r + dirs[i] * j;
            int nc = c + dcs[i] * j;
            if (nr < 0 || nr >= 8 || nc < 0 || nc >= 8) break;
            char target = board[nr][nc];
            if (target == ' ') {
                moves.push_back({r, c, nr, nc, piece});
            } else {
                if (isupper(target) != isWhite) {
                    moves.push_back({r, c, nr, nc, piece});
                }
                break;
            }
        }
    }
}

void ChessEngine::getQueenMoves(std::vector<Move>& moves, int r, int c) {
    getRookMoves(moves, r, c); // Ratu bergerak seperti Benteng + Gajah
    getBishopMoves(moves, r, c);
}

void ChessEngine::getKingMoves(std::vector<Move>& moves, int r, int c) {
    char piece = board[r][c];
    bool isWhite = isupper(piece);
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue;
            int nr = r + dr;
            int nc = c + dc;
            if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
                char target = board[nr][nc];
                if (target == ' ' || (isupper(target) != isWhite)) {
                    moves.push_back({r, c, nr, nc, piece});
                }
            }
        }
    }
}