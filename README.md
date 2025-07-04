PERSIAPAN DAN INSTALASI
Anda hanya memerlukan satu hal utama: Compiler C++.

Untuk Windows:

Install MinGW-w64. Cara termudah adalah melalui MSYS2.

Buka website MSYS2 dan ikuti instruksi instalasinya.

Setelah terinstal, buka terminal MSYS2 dan jalankan perintah: pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain

Tambahkan C:\msys64\ucrt64\bin ke PATH environment variable Anda.

Untuk memeriksa, buka Command Prompt (CMD) dan ketik g++ --version.

Untuk Linux (Debian/Ubuntu):
Buka terminal dan jalankan:

Bash:

sudo apt update
sudo apt install build-essential

Untuk macOS:
Install Xcode Command Line Tools. Buka terminal dan jalankan:

Bash:

xcode-select --install
