#include <iostream>
#include <string>
#include "Board.h"
#include "Engine.h"

// Fungsi untuk mengubah input user (e.g., "e2e4") menjadi struct Move
Move parse_move(const std::string& move_str) {
    Move m;
    m.from_col = move_str[0] - 'a';
    m.from_row = '8' - move_str[1];
    m.to_col = move_str[2] - 'a';
    m.to_row = '8' - move_str[3];
    return m;
}

int main() {
    Board board;
    Engine engine;

    // Atur kedalaman pencarian AI. Semakin tinggi, semakin pintar tapi semakin lambat.
    // 3 atau 4 adalah awal yang baik.
    const int AI_SEARCH_DEPTH = 3;

    while (true) {
        board.print_board();
        
        if(board.generate_legal_moves().empty()){
            std::cout << "Game Over!" << std::endl;
            break;
        }

        Move user_move;
        if (board.is_white_turn) { // Giliran pemain
            std::string move_str;
            std::cout << "Masukkan langkah Anda (contoh: e2e4): ";
            std::cin >> move_str;
            
            // Validasi input sederhana
            if (move_str.length() != 4) {
                std::cout << "Format langkah tidak valid. Coba lagi." << std::endl;
                continue;
            }
            user_move = parse_move(move_str);
            // NOTE: Di sini tidak ada validasi apakah langkah user legal atau tidak.
            // Untuk aplikasi nyata, ini harus ditambahkan.

        } else { // Giliran komputer
            std::cout << "\nKomputer sedang berpikir..." << std::endl;
            user_move = engine.find_best_move(board, AI_SEARCH_DEPTH);
            std::cout << "Komputer bergerak: " << user_move.to_string() << std::endl;
        }

        board.make_move(user_move);
    }

    return 0;
}