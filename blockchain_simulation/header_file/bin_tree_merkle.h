#ifndef BIN_TREE_MERKLE_H
#define BIN_TREE_MERKLE_H

#include "common_type.h"
#include "transaction.h"

typedef struct merkle_node {
    char hash[HASH_SIZE];
    char label[50];
    bool is_tainted;
    struct merkle_node *left;
    struct merkle_node *right;
} merkle_node;

merkle_node* create_merkle_node(char* hash, char* label, bool is_tainted);
void generate_tx_hash(tx* transaction);
merkle_node* build_merkle_tree(tx transactions[], int count);
void print_merkle_tree_visual(merkle_node* node, int level, char* prefix);
void free_merkle_tree(merkle_node* node);

#endif