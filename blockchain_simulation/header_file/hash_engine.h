#ifndef HASH_ENGINE_H
#define HASH_ENGINE_H

// Include tipe data umum yang kita butuhkan
#include "common_type.h"

// Struct untuk menyimpan data terkait simulasi hash
typedef struct hash_data_string
{
    char current_data[MAX_STRING];
    char current_hash[HASH_SIZE];
    char original_hash[HASH_SIZE];
} hash_data_string;

// === FUNGSI PUBLIK YANG BISA DIPANGGIL DARI MAIN ===
// Fungsi utama untuk menjalankan seluruh simulasi hash
void hash_simulation();

/* === FUNGSI INTERNAL (bisa juga dideklarasikan di .c saja jika tidak ingin diakses dari luar) === */ 
// Fungsi yang dipakai hanya untuk menu khusus simulasi hash
void hash_menu_string(hash_data_string *data_string); 

// Fungsi untuk menghasilkan hash dari sebuah string
void hash_generator_string(const char* input_string, char* output_hash, int status_process); // Fungsi untuk mengubah string menjadi hash
void hash_generator_tx(const char *input, char *output); // Fungsi untuk mengubah string menjadi hash 2
void change_data_string(hash_data_string *data_string); // Fungsi untuk mengubah data
void show_process_string(hash_data_string *data_string); // Fungsi untuk menampilkan proses hashing



#endif