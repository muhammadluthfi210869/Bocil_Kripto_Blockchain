#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "common_type.h"

typedef struct {
    int num;
    int transfer_id;
    int sender_id;
    char sender_name[MAX_NAME];
    unsigned int transfer_amount;
    int receiver_id;
    char receiver_name[MAX_NAME];
    char tx_hash[HASH_SIZE];
    bool is_edited;
} tx;

#endif