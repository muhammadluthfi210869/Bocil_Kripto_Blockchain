#ifndef BLOCKCHAIN_TX_H
#define BLOCKCHAIN_TX_H

#include "common_type.h"
#include "block_tx.h"

void blockchain_tx_sim();
void blockchain_tx_menu(address_tx genesis);
void print_blockchain_tx_status(address_tx genesis);
void print_block_tx_data(address_tx block);
address_tx find_tx_block_by_id(address_tx genesis, int id);
void insert_new_tx_block(address_tx *genesis);
void delete_tx_block_from_id(address_tx *genesis, int id);

void edit_selected_tx_block(address_tx block);

void save_blockchain_tx_to_file(address_tx genesis);
void load_blockchain_tx_from_file(address_tx *genesis);
void free_blockchain_tx(address_tx *genesis);
void validate_entire_tx_chain(address_tx genesis);
char* concanate_for_tx_block(address_tx block, bool for_nonce);
void calculate_tx_block_hash(address_tx block);
void calculate_tx_block_nonce(address_tx block);
void mining_hash_for_block(address_tx block);
void handle_edit_transaction_in_block(address_tx block);
void view_merkle_tree_for_block(address_tx block);
void transaction_submenu(address_tx block);
bool apply_transaction_to_database(tx transaction, bool is_reversal);

#endif