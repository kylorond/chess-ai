#include <iostream>
#include "ChessEngine.h"

void printMove(const Move& move) {
    std::cout << (char)('a' + move.start_col) << 8 - move.start_row
              << (char)('a' + move.end_col) << 8 - move.end_row;
}

int main() {
    ChessEngine engine;
    bool playerIsWhite = true; // Player memainkan Putih
    bool isPlayerTurn = true;
    int searchDepth = 4; // Tingkat kedalaman pencarian. Semakin tinggi, semakin pintar & lambat. 3-4 adalah awal yang baik.

    while (true) {
        engine.printBoard();
        if (isPlayerTurn) {
            std::cout << "Giliran Anda (Putih). Masukkan langkah (contoh: e2e4): ";
            std::string moveStr;
            std::cin >> moveStr;

            try {
                Move playerMove = engine.parseMove(moveStr);
                if (engine.isMoveValid(playerMove, playerIsWhite)) {
                    engine.makeMove(playerMove);
                    isPlayerTurn = false;
                } else {
                    std::cout << "Langkah tidak sah! Coba lagi." << std::endl;
                }
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
        } else {
            std::cout << "Giliran Komputer (Hitam)... berpikir..." << std::endl;
            Move bestMove = engine.getBestMove(searchDepth, !playerIsWhite);
            engine.makeMove(bestMove);
            std::cout << "Komputer melangkah: ";
            printMove(bestMove);
            std::cout << std::endl;
            isPlayerTurn = true;
        }
    }

    return 0;
}