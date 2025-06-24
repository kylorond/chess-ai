# Makefile untuk Proyek Catur C++

# Compiler yang digunakan
CXX = g++

# Flag untuk compiler (misal: standar C++11, warning)
CXXFLAGS = -std=c++11 -Wall

# Nama file executable yang akan dihasilkan
TARGET = ChessEngine

# Semua file source .cpp
SRCS = main.cpp Board.cpp Engine.cpp

# Mengubah daftar .cpp menjadi .o (file objek)
OBJS = $(SRCS:.cpp=.o)

# Aturan default: apa yang harus dilakukan jika hanya mengetik 'make'
all: $(TARGET)

# Aturan untuk membuat file executable dari file-file objek
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Aturan umum untuk membuat file .o dari file .cpp
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Aturan untuk membersihkan file hasil kompilasi
clean:
	rm -f $(TARGET) $(OBJS)