#ifndef BLOCK_TX_H
#define BLOCK_TX_H

#include "common_type.h"
#include "transaction.h"
#include "bin_tree_merkle.h"

typedef struct block_tx_node *address_tx;
typedef struct block_tx_node {
    int id_block_tx;
    int nonce_block_tx;
    tx current_transactions[MAX_TRANSACTIONS_IN_BLOCK];
    int transaction_count;
    merkle_node *merkle_root;
    char prev_hash_block_str[HASH_SIZE];
    char original_hash_block_str[HASH_SIZE];
    char current_hash_block_str[HASH_SIZE];
    address_tx prev_block;
    address_tx next_block;
} block_tx_node;

void block_tx_simulation(); // Simulasi single block

#endif