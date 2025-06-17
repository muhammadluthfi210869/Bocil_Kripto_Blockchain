#ifndef QUEUE_MEMPOOL_H
#define QUEUE_MEMPOOL_H

#include "common_type.h"
#include "transaction.h"
#include "block_tx.h"

typedef struct mempool_node { 
    tx transaction; 
    struct mempool_node *next; 
} mempool_node;

extern mempool_node *mempool_head, *mempool_tail;
extern int mempool_size;

void mempool_menu(address_tx block_target);
void enqueue_mempool(tx new_tx, bool create_log);
tx dequeue_mempool();
void add_new_transaction_to_mempool();
void print_mempool();
void view_transaction_detail();
bool delete_from_mempool_by_id(int tx_id, bool create_log);
void handle_mining_from_mempool(address_tx block_target);

#endif