#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>  // Untuk fungsi getch()

// Konstanta
#define MAX_NAME 50       // Panjang maksimum nama pengirim/penerima
#define HASH_SIZE 13      // 12 karakter hash + null terminator

/*
 * ===== ABSTRACT DATA TYPE (ADT) BLOCKCHAIN =====
 */

// ADT Block - Struktur data untuk blok dalam blockchain
typedef struct Block {
    int index;                  // Indeks blok dalam rantai
    char sender[MAX_NAME];      // Nama pengirim
    char receiver[MAX_NAME];    // Nama penerima
    int amount;                 // Jumlah transaksi
    char prev_hash[HASH_SIZE];  // Hash dari blok sebelumnya
    char hash[HASH_SIZE];       // Hash dari blok saat ini
    struct Block *next;         // Pointer ke blok berikutnya
} Block;

// ADT Blockchain - Struktur data untuk blockchain
typedef struct {
    Block *head;  // Pointer ke blok pertama (genesis)
    int size;     // Jumlah blok dalam blockchain
} Blockchain;

// Prototipe Fungsi ADT Blockchain
// Operasi Dasar
Blockchain* createBlockchain();                  // Membuat blockchain baru dengan blok genesis
void destroyBlockchain(Blockchain *bc);          // Membersihkan memori blockchain
Block* createBlock(int index, const char *sender, const char *receiver, int amount, const char *prev_hash); // Membuat blok baru
void addBlock(Blockchain *bc, const char *sender, const char *receiver, int amount); // Menambahkan blok ke blockchain
Block* getBlockByIndex(Blockchain *bc, int index); // Mendapatkan blok berdasarkan indeks

// Operasi Tampilan dan Verifikasi
void displayBlockchain(Blockchain *bc);          // Menampilkan seluruh blockchain
int verifyBlockchain(Blockchain *bc);            // Memverifikasi integritas blockchain
void tamperBlock(Blockchain *bc, int index, const char *new_sender, const char *new_receiver, int new_amount); // Memodifikasi data blok
void mineBlock(Blockchain *bc, int index);       // Melakukan mining ulang pada blok setelah data diubah

// Fungsi Utilitas
void calculateHash(const char *prev_hash, const char *sender, const char *receiver, int amount, char *hash_result); // Menghitung hash
void clearScreen();                             // Membersihkan layar
void displayMenu();                             // Menampilkan menu
void pauseScreen();                             // Jeda layar

/* ===== IMPLEMENTASI FUNGSI ===== */

// Fungsi untuk menghitung hash berdasarkan data blok
void calculateHash(const char *prev_hash, const char *sender, const char *receiver, int amount, char *hash_result) {
    // Gabungkan semua data menjadi satu string
    char combined_data[512];
    sprintf(combined_data, "%s|%s|%s|%d", prev_hash, sender, receiver, amount);
    
    // Inisialisasi array digest
    int digest[12] = {0};
    
    // Proses setiap karakter dan distribusikan ke array digest
    int len = strlen(combined_data);
    for (int i = 0; i < len; i++) {
        digest[i % 12] += (int)combined_data[i];
    }
    
    // Konversi nilai digest ke karakter yang dapat dibaca
    for (int i = 0; i < 12; i++) {
        int value = digest[i] % 62;  // Batasi ke 62 karakter (A-Z, a-z, 0-9)
        
        if (value < 26) {
            // A-Z (ASCII 65-90)
            hash_result[i] = 'A' + value;
        } else if (value < 52) {
            // a-z (ASCII 97-122)
            hash_result[i] = 'a' + (value - 26);
        } else {
            // 0-9 (ASCII 48-57)
            hash_result[i] = '0' + (value - 52);
        }
    }
    
    // Tambahkan null terminator
    hash_result[12] = '\0';
}

// Fungsi untuk membuat blok baru
Block* createBlock(int index, const char *sender, const char *receiver, int amount, const char *prev_hash) {
    Block *new_block = (Block*)malloc(sizeof(Block));
    if (new_block == NULL) {
        printf("Error: Gagal mengalokasikan memori untuk blok baru\n");
        exit(1);
    }
    
    new_block->index = index;
    strncpy(new_block->sender, sender, MAX_NAME - 1);
    new_block->sender[MAX_NAME - 1] = '\0';
    
    strncpy(new_block->receiver, receiver, MAX_NAME - 1);
    new_block->receiver[MAX_NAME - 1] = '\0';
    
    new_block->amount = amount;
    
    strncpy(new_block->prev_hash, prev_hash, HASH_SIZE - 1);
    new_block->prev_hash[HASH_SIZE - 1] = '\0';
    
    // Hitung hash untuk blok ini
    calculateHash(prev_hash, sender, receiver, amount, new_block->hash);
    
    new_block->next = NULL;
    
    return new_block;
}

// Fungsi untuk membuat blockchain baru dengan blok genesis
Blockchain* createBlockchain() {
    Blockchain *bc = (Blockchain*)malloc(sizeof(Blockchain));
    if (bc == NULL) {
        printf("Error: Gagal mengalokasikan memori untuk blockchain\n");
        exit(1);
    }
    
    // Buat blok genesis
    Block *genesis = createBlock(0, "Genesis", "Genesis", 0, "000000000000");
    
    bc->head = genesis;
    bc->size = 1;
    
    return bc;
}

// Fungsi untuk menambahkan blok baru ke blockchain
void addBlock(Blockchain *bc, const char *sender, const char *receiver, int amount) {
    if (bc == NULL || bc->head == NULL) {
        printf("Error: Blockchain tidak diinisialisasi dengan benar\n");
        return;
    }
    
    // Temukan blok terakhir
    Block *current = bc->head;
    while (current->next != NULL) {
        current = current->next;
    }
    
    // Buat blok baru
    Block *new_block = createBlock(bc->size, sender, receiver, amount, current->hash);
    
    // Tambahkan ke akhir blockchain
    current->next = new_block;
    bc->size++;
    
    printf("Blok baru ditambahkan: %d\n", new_block->index);
}

// Fungsi untuk memverifikasi integritas blockchain
int verifyBlockchain(Blockchain *bc) {
    if (bc == NULL || bc->head == NULL) {
        printf("Error: Blockchain tidak diinisialisasi dengan benar\n");
        return 0;
    }
    
    Block *current = bc->head->next;  // Mulai dari blok setelah genesis
    Block *previous = bc->head;
    
    while (current != NULL) {
        // Verifikasi prev_hash
        if (strcmp(current->prev_hash, previous->hash) != 0) {
            printf("WARNING: Integritas blockchain rusak pada blok %d!\n", current->index);
            printf("  Hash sebelumnya tidak cocok!\n");
            return 0;
        }
        
        // Verifikasi hash saat ini
        char calculated_hash[HASH_SIZE];
        calculateHash(current->prev_hash, current->sender, current->receiver, current->amount, calculated_hash);
        
        if (strcmp(current->hash, calculated_hash) != 0) {
            printf("WARNING: Integritas blockchain rusak pada blok %d!\n", current->index);
            printf("  Hash saat ini tidak cocok!\n");
            return 0;
        }
        
        previous = current;
        current = current->next;
    }
    
    printf("Blockchain terverifikasi: Semua blok valid\n");
    return 1;
}

// Fungsi untuk menampilkan seluruh blockchain
void displayBlockchain(Blockchain *bc) {
    if (bc == NULL || bc->head == NULL) {
        printf("Blockchain kosong\n");
        return;
    }
    
    Block *current = bc->head;
    
    printf("\n===== BLOCKCHAIN =====\n");
    while (current != NULL) {
        printf("\nBlok #%d\n", current->index);
        printf("Pengirim: %s\n", current->sender);
        printf("Penerima: %s\n", current->receiver);
        printf("Jumlah: %d\n", current->amount);
        printf("Previous Hash: %s\n", current->prev_hash);
        printf("Hash: %s\n", current->hash);
        
        current = current->next;
    }
    printf("\n=====================\n");
}

// Fungsi untuk memodifikasi data blok (untuk demonstrasi perusakan integritas)
void tamperBlock(Blockchain *bc, int index, const char *new_sender, const char *new_receiver, int new_amount) {
    if (bc == NULL || bc->head == NULL) {
        printf("Blockchain kosong\n");
        return;
    }
    
    Block *current = bc->head;
    
    // Cari blok dengan index yang sesuai
    while (current != NULL && current->index != index) {
        current = current->next;
    }
    
    if (current == NULL) {
        printf("Blok dengan index %d tidak ditemukan\n", index);
        return;
    }
    
    // Modifikasi data blok
    printf("\nMemodifikasi blok %d:\n", index);
    printf("  Pengirim: %s -> %s\n", current->sender, new_sender);
    printf("  Penerima: %s -> %s\n", current->receiver, new_receiver);
    printf("  Jumlah: %d -> %d\n", current->amount, new_amount);
    
    strncpy(current->sender, new_sender, MAX_NAME - 1);
    current->sender[MAX_NAME - 1] = '\0';
    
    strncpy(current->receiver, new_receiver, MAX_NAME - 1);
    current->receiver[MAX_NAME - 1] = '\0';
    
    current->amount = new_amount;
    
    printf("Blok %d telah dimodifikasi (tanpa menghitung ulang hash)\n", index);
}

// Fungsi untuk mining ulang blok setelah data diubah
void mineBlock(Blockchain *bc, int index) {
    if (bc == NULL || bc->head == NULL) {
        printf("Blockchain kosong\n");
        return;
    }
    Block *current = bc->head;
    Block *prev = NULL;
    // Cari blok dengan index yang sesuai
    while (current != NULL && current->index != index) {
        prev = current;
        current = current->next;
    }
    if (current == NULL) {
        printf("Blok dengan index %d tidak ditemukan\n", index);
        return;
    }
    // Mining ulang blok ini dan semua blok setelahnya
    while (current != NULL) {
        if (prev == NULL) {
            // Genesis block
            calculateHash(current->prev_hash, current->sender, current->receiver, current->amount, current->hash);
        } else {
            strncpy(current->prev_hash, prev->hash, HASH_SIZE - 1);
            current->prev_hash[HASH_SIZE - 1] = '\0';
            calculateHash(current->prev_hash, current->sender, current->receiver, current->amount, current->hash);
        }
        prev = current;
        current = current->next;
    }
    printf("Mining selesai! Hash blok dan rantai setelahnya telah diperbarui.\n");
}

// Fungsi untuk membersihkan memori blockchain
void destroyBlockchain(Blockchain *bc) {
    if (bc == NULL) {
        return;
    }
    
    Block *current = bc->head;
    Block *next;
    
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    
    free(bc);
}

// Fungsi untuk mendapatkan blok berdasarkan indeks
Block* getBlockByIndex(Blockchain *bc, int index) {
    if (bc == NULL || bc->head == NULL) {
        return NULL;
    }
    
    Block *current = bc->head;
    while (current != NULL) {
        if (current->index == index) {
            return current;
        }
        current = current->next;
    }
    
    return NULL;
}

// Fungsi untuk membersihkan layar
void clearScreen() {
    system("cls"); // Untuk Windows
    // system("clear"); // Untuk Unix/Linux
}

// Fungsi untuk jeda layar
void pauseScreen() {
    printf("\nTekan tombol apa saja untuk melanjutkan...");
    getch();
}

// Fungsi untuk menampilkan menu
void displayMenu() {
    clearScreen();
    printf("\n===== MENU BLOCKCHAIN =====\n");
    printf("1. Tambah blok baru\n");
    printf("2. Tampilkan blockchain\n");
    printf("3. Verifikasi integritas blockchain\n");
    printf("4. Modifikasi data blok\n");
    printf("5. Mining blok (perbaiki hash)\n");
    printf("6. Keluar\n");
    printf("Pilihan: ");
}

int main() {
    // Inisialisasi blockchain dengan blok genesis
    Blockchain *bc = createBlockchain();
    
    // Tambahkan beberapa blok transaksi awal
    addBlock(bc, "Alice", "Bob", 100);
    addBlock(bc, "Bob", "Charlie", 50);
    addBlock(bc, "Charlie", "Dave", 75);
    
    int choice;
    char sender[MAX_NAME];
    char receiver[MAX_NAME];
    int amount;
    int block_index;
    
    do {
        displayMenu();
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: // Tambah blok baru
                clearScreen();
                printf("\n===== TAMBAH BLOK BARU =====\n");
                printf("Masukkan data blok baru:\n");
                printf("Pengirim: ");
                scanf("%s", sender);
                printf("Penerima: ");
                scanf("%s", receiver);
                printf("Jumlah: ");
                scanf("%d", &amount);
                
                addBlock(bc, sender, receiver, amount);
                printf("\nBlok baru berhasil ditambahkan!");
                pauseScreen();
                break;
                
            case 2: // Tampilkan blockchain
                clearScreen();
                displayBlockchain(bc);
                pauseScreen();
                break;
                
            case 3: // Verifikasi integritas
                clearScreen();
                printf("\n===== VERIFIKASI INTEGRITAS BLOCKCHAIN =====\n");
                verifyBlockchain(bc);
                pauseScreen();
                break;
                
            case 4: // Modifikasi data blok
                clearScreen();
                printf("\n===== MODIFIKASI DATA BLOK =====\n");
                displayBlockchain(bc);
                
                printf("\nMasukkan indeks blok yang ingin dimodifikasi: ");
                scanf("%d", &block_index);
                
                if (getBlockByIndex(bc, block_index) == NULL) {
                    printf("Blok dengan indeks %d tidak ditemukan!\n", block_index);
                    pauseScreen();
                    break;
                }
                
                printf("\nMasukkan data baru:\n");
                printf("Pengirim baru: ");
                scanf("%s", sender);
                printf("Penerima baru: ");
                scanf("%s", receiver);
                printf("Jumlah baru: ");
                scanf("%d", &amount);
                
                tamperBlock(bc, block_index, sender, receiver, amount);
                printf("\nData blok telah dimodifikasi. Silakan verifikasi integritas untuk melihat peringatan.\n");
                pauseScreen();
                break;
            case 5: // Mining blok
                clearScreen();
                printf("\n===== MINING BLOK =====\n");
                displayBlockchain(bc);
                printf("\nMasukkan indeks blok yang ingin di-mining ulang: ");
                scanf("%d", &block_index);
                if (getBlockByIndex(bc, block_index) == NULL) {
                    printf("Blok dengan indeks %d tidak ditemukan!\n", block_index);
                    pauseScreen();
                    break;
                }
                mineBlock(bc, block_index);
                pauseScreen();
                break;
            case 6: // Keluar
                clearScreen();
                printf("\n===== TERIMA KASIH =====\n");
                printf("Keluar dari program...\n");
                break;
            default:
                printf("\nPilihan tidak valid!\n");
                pauseScreen();
        }
        
    } while (choice != 6);
    
    // Bersihkan memori
    destroyBlockchain(bc);
    
    return 0;
}