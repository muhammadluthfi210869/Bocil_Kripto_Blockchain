#ifndef STACK_LOG_TRANSACTION_H
#define STACK_LOG_TRANSACTION_H

#include "common_type.h"
#include "transaction.h"

typedef enum { LOG_ADD, LOG_DELETE, LOG_MINING } log_action_type;
typedef struct log_node { 
    log_action_type action; 
    tx transaction_data; 
    int mining_count; 
    struct log_node *next; 
} log_node;

extern log_node *undo_stack, *redo_stack;

void push_log(log_node **stack, log_action_type action, tx data, int mining_count);
log_node* pop_log(log_node **stack);
void clear_stack(log_node **stack);
void handle_undo();
void handle_redo();
void view_history_log();

#endif