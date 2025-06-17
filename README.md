# Simulasi Blockchain v1.4

Sebuah program simulasi konsep-konsep dasar teknologi blockchain yang dibuat untuk tujuan edukasi, menggunakan implementasi dari berbagai struktur data.

**Kontributor:**
* Lukman Ahmad (241524050)
* Muhammad Luthfi (241524054)

---

## 📜 Tentang Program

Program ini adalah simulasi edukatif yang bertujuan untuk mendemonstrasikan cara kerja beberapa fitur utama dalam teknologi blockchain melalui implementasi algoritma dan struktur data. Program ini tidak merepresentasikan 100% sistem blockchain di dunia nyata, karena beberapa fitur disederhanakan untuk fokus pada pemahaman konsep dasarnya.

## ✨ Fitur Utama

Program ini dibagi menjadi beberapa modul simulasi utama:

* **Simulasi Hash**: Mendemonstrasikan bagaimana fungsi hash mengubah data input menjadi output dengan panjang tetap dan bagaimana perubahan kecil pada input (efek avalanche) memengaruhi hash output.
* **Simulasi Blok & Blockchain (String)**: Simulasi dasar di mana setiap blok hanya berisi data string. Modul ini menunjukkan bagaimana blok-blok saling terhubung dan bagaimana mengubah satu blok akan menginvalidasi blok-blok berikutnya dalam rantai.
* **Simulasi Blok & Blockchain (Transaksi)**: Simulasi inti dari program ini, di mana setiap blok berisi sekumpulan transaksi. Fitur ini mencakup:
    * **Manajemen Pengguna**: Sistem untuk membuat, mengubah, mencari, dan menyimpan data pengguna yang terlibat dalam transaksi.
    * **Mempool (Queue)**: Sebuah area tunggu (antrian) untuk transaksi yang belum diproses. Transaksi baru masuk ke sini sebelum ditambahkan ke blok melalui proses mining.
    * **Log Transaksi (Stack)**: Implementasi fitur Undo/Redo untuk aksi di dalam Mempool (seperti menambah atau menghapus transaksi) menggunakan struktur data Stack.
    * **Merkle Tree**: Setiap blok transaksi memiliki Merkle Tree yang dibangun dari hash semua transaksinya untuk verifikasi data yang efisien dan aman.
    * **Proses Mining**: Simulasi proses validasi blok baru dengan menghitung *nonce* dan *hash* yang valid, lalu memindahkan transaksi dari Mempool ke dalam blok.
* **Teori Blockchain**: Modul edukatif yang berisi penjelasan mendalam tentang semua konsep yang disimulasikan dalam program ini, mulai dari hash, blok, nonce, hingga Merkle Tree.
* **Penyimpanan Data**: Kemampuan untuk menyimpan dan memuat kembali seluruh status blockchain dan data pengguna dari file `.dat`, sehingga progres tidak hilang.

## 🏗️ Struktur Data yang Digunakan

Program ini mengimplementasikan beberapa struktur data fundamental:

* **Linked List (Doubly)**: Digunakan untuk membangun rantai blockchain, di mana setiap blok terhubung ke blok sebelum dan sesudahnya.
* **Queue (Berbasis Linked List)**: Diimplementasikan sebagai **Mempool** untuk menampung transaksi yang menunggu untuk di-mining, mengikuti prinsip First-In, First-Out (FIFO).
* **Stack (Berbasis Linked List)**: Digunakan untuk mencatat riwayat aksi di Mempool (`undo_stack` dan `redo_stack`), memungkinkan operasi Undo dan Redo.
* **Binary Tree**: Diimplementasikan sebagai **Merkle Tree** di dalam setiap blok transaksi untuk meringkas dan memverifikasi seluruh transaksi di dalamnya.
* **Array**: Digunakan untuk mengelola database pengguna (`user_database`) secara sederhana.

## 📁 Struktur Proyek

Proyek ini terdiri dari beberapa file header (`.h`) dan implementasi (`.c`) yang saling terhubung:

| File                  | Deskripsi                                                                 |
| --------------------- | ------------------------------------------------------------------------- |
| `blockchain_simulation.c` | File `main` yang menjadi titik masuk dan pengatur menu utama program.      |
| `common_type.h`       | Berisi definisi tipe data, konstanta global, dan *include* library standar. |
| `menu.h` / `.c`         | Mengelola semua fungsi terkait tampilan antarmuka pengguna (UI).         |
| `hash_engine.h` / `.c`  | Implementasi algoritma hash kustom yang digunakan di seluruh simulasi.   |
| `transaction.h`       | Mendefinisikan struktur data untuk sebuah transaksi (`tx`).                   |
| `user_management.h` / `.c` | Mengelola data pengguna, termasuk saldo dan operasi CRUD.            |
| `queue_mempool.h` / `.c`  | Implementasi Mempool menggunakan struktur data Queue.                 |
| `stack_log_transaction.h` / `.c` | Implementasi fitur Undo/Redo di Mempool menggunakan Stack. |
| `bin_tree_merkle.h` / `.c` | Implementasi Merkle Tree dari sekumpulan transaksi.               |
| `block_tx.h` / `.c`       | Mendefinisikan struktur blok transaksi dan simulasi blok tunggal.     |
| `blockchain_tx.h` / `.c`  | Mengelola logika untuk keseluruhan rantai blok transaksi.          |
| `block_string.h` / `.c`   | Definisi dan logika untuk simulasi blok string tunggal.           |
| `blockchain_string.h` / `.c` | Mengelola logika untuk keseluruhan rantai blok string.        |
| `theory.h` / `.c`         | Berisi semua teks penjelasan untuk modul Teori Blockchain.            |

Struktur File
blockchain_simulation
|
|_header_file (isinya file .h)
|       
|_src_code (isinya file .c)

## 🚀 Cara Menjalankan

1.  **Pastikan Anda memiliki C Compiler (seperti GCC).**
2.  **Kompilasi semua file `.c` menjadi satu file eksekusi.**
    Buka terminal atau command prompt, navigasi ke direktori proyek, dan jalankan perintah berikut:
    ```bash
    gcc src_code/*.c -Iheader_file -Wall -o blockchain_simulation.exe
    ```
3.  **Jalankan program:**
    * Di Windows:
        ```bash
        .\blockchain_simulation.exe
        ```
    * Di Linux/macOS:
        ```bash
        ./blockchain_simulation
        ```
4.  **Program akan menampilkan menu utama** dan siap untuk digunakan.

---
