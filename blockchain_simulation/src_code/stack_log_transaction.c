#include "stack_log_transaction.h"
#include "menu.h"
#include "queue_mempool.h"

log_node *undo_stack = NULL;
log_node *redo_stack = NULL;

void push_log(log_node **stack, log_action_type action, tx data, int mining_count)
{
    log_node *new_node = (log_node *)malloc(sizeof(log_node));
    new_node->action = action;
    new_node->transaction_data = data;
    new_node->mining_count = mining_count;
    new_node->next = *stack;
    *stack = new_node;
}
log_node *pop_log(log_node **stack)
{
    if (*stack == NULL)
        return NULL;
    log_node *temp = *stack;
    *stack = (*stack)->next;
    temp->next = NULL;
    return temp;
}
void clear_stack(log_node **stack)
{
    while (*stack != NULL)
    {
        log_node *temp = pop_log(stack);
        free(temp);
    }
}

void handle_undo()
{
    log_node *last_action = pop_log(&undo_stack);
    if (!last_action)
    {
        printf("Tidak ada aksi untuk di-undo!\n");
        pause_screen();
        return;
    }
    switch (last_action->action)
    {
    case LOG_ADD:
        delete_from_mempool_by_id(last_action->transaction_data.transfer_id, false);
        printf("Undo: Menghapus TX-%d.\n", last_action->transaction_data.transfer_id);
        break;
    case LOG_DELETE:
        enqueue_mempool(last_action->transaction_data, false);
        printf("Undo: Menambahkan kembali TX-%d.\n", last_action->transaction_data.transfer_id);
        break;
    case LOG_MINING:
        break;
    }
    push_log(&redo_stack, last_action->action, last_action->transaction_data, last_action->mining_count);
    free(last_action);
    pause_screen();
}

void handle_redo()
{
    log_node *last_undo = pop_log(&redo_stack);
    if (!last_undo)
    {
        printf("Tidak ada aksi untuk di-redo!\n");
        pause_screen();
        return;
    }
    switch (last_undo->action)
    {
    case LOG_ADD:
        enqueue_mempool(last_undo->transaction_data, true);
        printf("Redo: Menambahkan kembali TX-%d.\n", last_undo->transaction_data.transfer_id);
        break;
    case LOG_DELETE:
        delete_from_mempool_by_id(last_undo->transaction_data.transfer_id, true);
        printf("Redo: Menghapus kembali TX-%d.\n", last_undo->transaction_data.transfer_id);
        break;
    case LOG_MINING:
        break;
    }
    free(last_undo);
    pause_screen();
}

void view_history_log()
{
    clear_screen();
    printf("--- Mempool History (Newest First) ---\n");
    if (undo_stack == NULL)
    {
        printf("Tidak ada histori.\n");
    }
    else
    {
        log_node *current = undo_stack;
        while (current != NULL)
        {
            switch (current->action)
            {
            case LOG_ADD:
                printf("ADD:    Transaction TX-%d (%s -> %s, %u$) was added to mempool.\n", current->transaction_data.transfer_id, current->transaction_data.sender_name, current->transaction_data.receiver_name, current->transaction_data.transfer_amount);
                break;
            case LOG_DELETE:
                printf("DELETE: Transaction TX-%d was deleted from mempool.\n", current->transaction_data.transfer_id);
                break;
            case LOG_MINING:
                printf("MINING: %d transaction(s) were mined into a block.\n", current->mining_count);
                break;
            }
            current = current->next;
        }
    }
    pause_screen();
}