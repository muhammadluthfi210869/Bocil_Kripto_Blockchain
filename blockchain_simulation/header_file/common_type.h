#ifndef COMMON_TYPE_H
#define COMMON_TYPE_H

/*
ATURAN PENAMAAN KONSTANTA, FUNGSI, DAN VARIABEL
- LANGUAGE : ENGLISH / EN
- MAKRO / KONSTANTA : HURUF_KAPITAL_SEMUA_DEGNAN_UNDERSCORE EN
- FUNGSI : huruf_kecil_semua_dengan_underscore_jika_perlu (snake_case) EN
- VARIABEL : huruf_kecil_semua_dengan_underscore_jika_perlu (snake_case) EN
- KOMENTAR : Bahasa Inggris / Indo bebas
*/

// Include Package
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <conio.h>
#include <unistd.h>
#include <ctype.h>

// Color text Package
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define WHITE   "\x1b[0m"

//Regular background
#define BLACK_BG "\e[40m"
#define RED_BG "\e[41m"
#define GREEN_BG "\e[42m"
#define YELLOW_BG "\e[43m"
#define BLUE_BG "\e[44m"
#define MAGENTA_BG "\e[45m"
#define CYAN_BG "\e[46m"
#define WHITE_BG "\e[47m"

// User data
#define MAX_NAME 50       // Panjang maksimum nama pengirim/penerima
#define HASH_SIZE 13      // 12 karakter hash + null terminator
#define HASH_LEN 12       // Untuk menyimpan panjang Hash
#define NONCE_LEN 6       // Untuk menyimpan panjang Nonce
#define MAX_TRANSACTIONS_IN_BLOCK 8
#define USER_DATA_FILE "users.dat"
#define MAX_MEMPOOL_SIZE 100
#define MAX_USERS 50

// Hash Data 
#define MAX_STRING 256    // Panjang maksimum string

// Block Data 
#define FILENAME_STR "blockchain_string.dat" // Nama file untuk menyimpan data

#define BLOCK_DATA_FILE "blockchain_tx.dat"

// Boolean Data 
    #define TRUE 1
    #define FALSE 0
    #define boolean unsigned char

#endif