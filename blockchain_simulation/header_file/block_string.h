#ifndef BLOCK_STRING_H
#define BLOCK_STRING_H
/*
Header untuk blok string tunggal
*/

// Include ADT
#include "common_type.h"

// Header tambahan
#include "hash_engine.h"

// Deklarasi pointer ke node blok string untuk kemudahan penulisan 
// Ini adalah gaya penulisan pointer dalam pengembangan program ini
typedef struct block_string_node *address_string; //Typedef (Don't delete this, use for easy variable declare)
typedef struct block_string_node
{
    int id_block_str; // ID Unik blok
    int nonce_block_str; // Nilai "Nonce" (dalam simulasi hanyalah angka acak seperti hash)
    char current_data_block_str[MAX_STRING]; // Konten string dari blok 
    char prev_hash_block_str[HASH_SIZE]; // Hash dari blok sebelumnya 
    char original_hash_block_str[HASH_SIZE]; // Hash yang dianggap "valid" setelah mining
    char current_hash_block_str[HASH_SIZE]; // Hash yang dihitung dari perubahan data
    address_string prev_block; // Pointer ke blok sebelumnya
    address_string next_block; // Pointer ke blok berikutnya
} block_string_node; 

// Public Function (Bisa dipanggil di Main langsung)
void block_str_sim();

// Menu Handling
void block_str_menu(address_string block);

// Operation Function
void create_block_str(address_string *node_string);
void fill_block_str(address_string *node_string);
char *concanate_str(address_string block, boolean nonce_condition);
int calculate_nonce(address_string block);
void calculate_current_hash_string(address_string block);

void edit_id_block_str(address_string block);
void edit_nonce_block_str(address_string block);
void edit_data_block_str(address_string block);

void mining_block_str(address_string block);

#endif