#include "queue_mempool.h"
#include "menu.h"
#include "stack_log_transaction.h"
#include "user_management.h"

mempool_node *mempool_head = NULL;
mempool_node *mempool_tail = NULL;
int mempool_size = 0;

void mempool_menu(address_tx block_target)
{
    int choice;
    do
    {
        clear_screen();
        printf("--- Mempool (Target: Block #%d) ---\n", block_target->id_block_tx);
        print_mempool();
        printf("\nMenu:\n1. View Transaction Detail\n2. Add Transaction\n3. Delete Transaction\n4. Undo Last Action\n5. Redo Last Action\n6. Mining (Move TX to Block #%d)\n7. View History\n8. Back to Block Menu\n\n", block_target->id_block_tx);
        choice = get_integer_input("Your Option: ");
        switch (choice)
        {
        case 1:
            view_transaction_detail();
            break;
        case 2:
            add_new_transaction_to_mempool();
            break;
        case 3:
            int id = get_integer_input("Enter TX ID to delete: ");
            delete_from_mempool_by_id(id, true);
            pause_screen();
            break;
        case 4:
            handle_undo();
            break;
        case 5:
            handle_redo();
            break;
        case 6:
            handle_mining_from_mempool(block_target);
            break;
        case 7:
            view_history_log();
            break;
        case 8:
            break;
        default:
            printf("Pilihan tidak valid!\n");
        }
    } while (choice != 8);
}

void enqueue_mempool(tx new_tx, bool create_log)
{
    mempool_node *new_node = (mempool_node *)malloc(sizeof(mempool_node));
    new_node->transaction = new_tx;
    new_node->next = NULL;
    if (mempool_tail == NULL)
    {
        mempool_head = mempool_tail = new_node;
    }
    else
    {
        mempool_tail->next = new_node;
        mempool_tail = new_node;
    }
    mempool_size++;
    if (create_log)
    {
        push_log(&undo_stack, LOG_ADD, new_tx, 0);
        clear_stack(&redo_stack);
    }
}

tx dequeue_mempool()
{
    if (mempool_head == NULL)
    {
        tx empty_tx = {0};
        return empty_tx;
    }
    mempool_node *temp = mempool_head;
    tx dequeued_tx = temp->transaction;
    mempool_head = mempool_head->next;
    if (mempool_head == NULL)
    {
        mempool_tail = NULL;
    }
    free(temp);
    mempool_size--;
    return dequeued_tx;
}

void print_mempool()
{
    if (mempool_head == NULL)
    {
        printf("Mempool is empty.\n");
        return;
    }
    mempool_node *current = mempool_head;
    int i = 1;
    while (current != NULL)
    {
        printf("%d. TX[%d] -> ", i++, current->transaction.transfer_id);
        current = current->next;
    }
    printf("NULL\n");
}

void view_transaction_detail()
{
    if (mempool_size == 0)
    {
        printf("Mempool kosong.\n");
        pause_screen();
        return;
    }
    int tx_id = get_integer_input("Masukkan ID Transaksi yang ingin dilihat: ");
    mempool_node *current = mempool_head;
    while (current != NULL)
    {
        if (current->transaction.transfer_id == tx_id)
        {
            clear_screen();
            tx t = current->transaction;
            printf("--- Detail Transaksi TX-%d ---\n", t.transfer_id);
            printf("Pengirim: %s (ID: %d)\n", t.sender_name, t.sender_id);
            printf("Penerima: %s (ID: %d)\n", t.receiver_name, t.receiver_id);
            printf("Jumlah  : %u$\n", t.transfer_amount);
            pause_screen();
            return;
        }
        current = current->next;
    }
    printf("Transaksi dengan ID %d tidak ditemukan di mempool.\n", tx_id);
    pause_screen();
}

void handle_mining_from_mempool(address_tx block_target)
{
    int available_space = MAX_TRANSACTIONS_IN_BLOCK - block_target->transaction_count;
    if (available_space <= 0)
    {
        printf("Block target sudah penuh!\n");
        pause_screen();
        return;
    }
    if (mempool_size == 0)
    {
        printf("Mempool kosong!\n");
        pause_screen();
        return;
    }
    int amount_to_mine = get_integer_input("Mining Amount: ");
    if (amount_to_mine <= 0 || amount_to_mine > available_space || amount_to_mine > mempool_size)
    {
        printf("Jumlah mining tidak valid!\n");
        pause_screen();
        return;
    }

    printf("Mining %d transaction(s) into block #%d...\n", amount_to_mine, block_target->id_block_tx);
    for (int i = 0; i < amount_to_mine; i++)
    {
        tx mined_tx = dequeue_mempool();
        apply_transaction_to_database(mined_tx, false);
        mined_tx.num = block_target->transaction_count + 1;
        mined_tx.is_edited = false;
        block_target->current_transactions[block_target->transaction_count++] = mined_tx;
        printf("TX-%d: ... SUCCESS\n", mined_tx.transfer_id);
    }
    printf("Mining selesai. Log history mempool dibersihkan.\n");
    clear_stack(&undo_stack);
    clear_stack(&redo_stack);
    pause_screen();
}

bool delete_from_mempool_by_id(int tx_id, bool create_log)
{
    mempool_node *current = mempool_head, *prev = NULL;
    while (current != NULL && current->transaction.transfer_id != tx_id)
    {
        prev = current;
        current = current->next;
    }
    if (current == NULL)
    {
        printf("Transaksi dengan ID %d tidak ditemukan.\n", tx_id);
        return false;
    }
    tx deleted_tx = current->transaction;
    if (create_log)
    {
        push_log(&undo_stack, LOG_DELETE, deleted_tx, 0);
        clear_stack(&redo_stack);
    }
    if (prev == NULL)
    {
        mempool_head = current->next;
    }
    else
    {
        prev->next = current->next;
    }
    if (current->next == NULL)
    {
        mempool_tail = prev;
    }
    printf("Transaksi TX-%d berhasil dihapus dari mempool.\n", current->transaction.transfer_id);
    free(current);
    mempool_size--;
    return true;
}

void add_new_transaction_to_mempool()
{
    clear_screen();
    printf("--- Add New Transaction ---\n");
    print_user_table(user_database, user_count);
    if (mempool_size >= MAX_MEMPOOL_SIZE)
    {
        printf("\nMempool sudah penuh!\n");
        pause_screen();
        return;
    }
    tx new_tx;
    new_tx.transfer_id = rand() % 9000 + 1000;
    new_tx.is_edited = false;
    new_tx.sender_id = get_integer_input("\nSender ID: ");
    db_user *sender = find_user_by_id(new_tx.sender_id);
    if (!sender)
    {
        printf("Sender ID tidak ditemukan!\n");
        pause_screen();
        return;
    }
    new_tx.receiver_id = get_integer_input("Receiver ID: ");
    db_user *receiver = find_user_by_id(new_tx.receiver_id);
    if (!receiver)
    {
        printf("Receiver ID tidak ditemukan!\n");
        pause_screen();
        return;
    }
    new_tx.transfer_amount = (unsigned int)get_integer_input("Amount: ");
    strcpy(new_tx.sender_name, sender->user_name);
    strcpy(new_tx.receiver_name, receiver->user_name);
    enqueue_mempool(new_tx, true);
    printf("\nTransaction TX-%d berhasil ditambahkan ke Mempool!\n", new_tx.transfer_id);
    pause_screen();
}