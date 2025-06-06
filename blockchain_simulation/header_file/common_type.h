#ifndef COMMON_TYPE_H
#define COMMON_TYPE_H

/*
ATURAN PENAMAAN KONSTANTA, FUNGSI, DAN VARIABEL
- LANGUAGE : ENGLISH
- MAKRO / KONSTANTA : HURUF_KAPITAL_SEMUA_DEGNAN_UNDERSCORE
- FUNGSI : huruf_kecil_semua_dengan_underscore_jika_perlu (snake_case)
- VARIABEL : huruf_kecil_semua_dengan_underscore_jika_perlu (snake_case)
- KOMENTAR : Bahasa Inggris / Indo bebas
*/

// Include Package
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>

// Color text Package
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define WHITE   "\x1b[0m"

// User data
    #define MAX_NAME 50       // Panjang maksimum nama pengirim/penerima
    #define HASH_SIZE 13      // 12 karakter hash + null terminator

// Hash Data 

// Block Data 

// Boolean Data 
    #define TRUE 1
    #define FALSE 0

#endif