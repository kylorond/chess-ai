#pragma once // Mencegah header di-include lebih dari sekali

#include <vector>
#include <string>
#include <map>

// Struktur untuk merepresentasikan sebuah langkah
struct Move {
    int start_row, start_col;
    int end_row, end_col;
    char piece;
};

class ChessEngine {
public:
    // Constructor untuk inisialisasi papan
    ChessEngine();

    // Mencetak papan ke konsol
    void printBoard();

    // Melakukan sebuah langkah di papan
    void makeMove(const Move& move);

    // Membatalkan langkah terakhir
    void undoMove(const Move& move, char capturedPiece);

    // Mendapatkan langkah terbaik untuk pemain saat ini
    Move getBestMove(int depth, bool isMaximizingPlayer);

    // Fungsi untuk mengubah notasi (misal: "e2e4") menjadi objek Move
    Move parseMove(const std::string& moveStr);

    // Memvalidasi apakah sebuah langkah dari user sah (versi sederhana)
    bool isMoveValid(const Move& move, bool isWhiteTurn);

private:
    std::vector<std::vector<char>> board;
    std::map<char, int> pieceValues;

    // Fungsi evaluasi untuk menilai posisi papan
    int evaluateBoard();

    // Algoritma Minimax dengan Alpha-Beta Pruning
    int minimax(int depth, int alpha, int beta, bool isMaximizingPlayer);

    // Menghasilkan semua langkah yang mungkin untuk pemain tertentu
    std::vector<Move> generateMoves(bool isMaximizingPlayer);

    // Fungsi bantuan untuk menghasilkan langkah untuk bidak tertentu
    void getPawnMoves(std::vector<Move>& moves, int r, int c, bool isWhite);
    void getRookMoves(std::vector<Move>& moves, int r, int c);
    void getKnightMoves(std::vector<Move>& moves, int r, int c);
    void getBishopMoves(std::vector<Move>& moves, int r, int c);
    void getQueenMoves(std::vector<Move>& moves, int r, int c);
    void getKingMoves(std::vector<Move>& moves, int r, int c);
};