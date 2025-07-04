# Makefile untuk kompilasi proyek catur

# Compiler
CXX = g++

# Flag compiler: -std=c++17 untuk fitur C++17, -Wall untuk semua warning, -O2 untuk optimisasi
CXXFLAGS = -std=c++17 -Wall -O2

# Nama file executable yang akan dihasilkan
TARGET = chess_app

# Semua file source .cpp
SOURCES = main.cpp ChessEngine.cpp

# Semua file header .h
HEADERS = ChessEngine.h

# Aturan default: kompilasi program
all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

# Aturan untuk membersihkan file hasil kompilasi
clean:
	rm -f $(TARGET)

# Menandai 'all' dan 'clean' sebagai phony targets
.PHONY: all clean