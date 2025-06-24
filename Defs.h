#ifndef DEFS_H
#define DEFS_H

#include <string>

// Struct untuk merepresentasikan satu langkah
// Menggunakan notasi koordinat (0-7)
struct Move {
    int from_row, from_col;
    int to_row, to_col;

    // Untuk mempermudah debugging
    std::string to_string() const {
        std::string s = "";
        s += (char)('a' + from_col);
        s += (char)('8' - from_row);
        s += (char)('a' + to_col);
        s += (char)('8' - to_row);
        return s;
    }
};

#endif // DEFS_H