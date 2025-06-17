#ifndef BLOCKCHAIN_STRING_H
#define BLOCKCHAIN_STRING_H

// Include ADT
#include "common_type.h"
#include "block_string.h"

// Public Function (Bisa dipanggil di Main langsung)
void blockchain_str_sim();

// Menu Handling
void blockchain_str_menu(address_string block);

// Operation function
address_string find_block_by_id(address_string genesis, int id);

void insert_new_block(address_string *genesis);
void delete_block_from_id(address_string *genesis);
void edit_selected_block(address_string block);
void edit_selected_block_menu(address_string block);
void validate_entire_chain(address_string genesis);
void print_blockchain_status(address_string genesis);

void create_initial_chain(address_string *genesis);
void save_blockchain_to_file(address_string genesis);
void load_blockchain_from_file(address_string *genesis);
void free_blockchain(address_string *genesis);

#endif