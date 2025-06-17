#include "blockchain_tx.h"
#include "menu.h"
#include "user_management.h"
#include "queue_mempool.h"
#include "bin_tree_merkle.h"
#include "hash_engine.h"

// Implementasi semua fungsi dari blockchain_tx.h
void blockchain_tx_sim()
{
    address_tx genesis = NULL;
    load_blockchain_tx_from_file(&genesis);
    int choice;
    do
    {
        clear_screen();
        validate_entire_tx_chain(genesis);
        blockchain_tx_menu(genesis);
        choice = get_integer_input("Your Option: ");

        switch (choice)
        {
        case 1:
            int id = get_integer_input("Enter Block ID to view/edit: ");
            address_tx target = find_tx_block_by_id(genesis, id);
            if (target)
                edit_selected_tx_block(target);
            else
            {
                printf("Block tidak ditemukan!\n");
                pause_screen();
            }
            break;
        case 2:
            insert_new_tx_block(&genesis);
            break;
        case 3:
            int id_del = get_integer_input("Enter Block ID to delete: ");
            delete_tx_block_from_id(&genesis, id_del);
            break;
        case 4:
            user_management_menu(genesis);
            break;
        case 5:
            save_blockchain_tx_to_file(genesis);
            save_users_to_file();
            break;
        case 6:
            break; // Exit
        default:
            printf("Pilihan tidak valid!\n");
            pause_screen();
        }
    } while (choice != 6);
    free_blockchain_tx(&genesis);
}

void blockchain_tx_menu(address_tx genesis)
{
    printf("==============================================================\n");
    printf("             BLOCKCHAIN TRANSACTION (TX) SIMULATION             \n");
    printf("==============================================================\n\n");
    if (genesis == NULL)
    {
        printf("%sBlockchain Kosong!!!%s\n\n", YELLOW, WHITE);
    }
    else
    {
        print_blockchain_tx_status(genesis);
    }
    printf("1. View/Edit Block by ID\n");
    printf("2. Insert New Block\n");
    printf("3. Delete Block by ID\n");
    printf("4. User Management\n");
    printf("5. Save All Data\n");
    printf("6. Exit Simulation\n\n");
}

void print_blockchain_tx_status(address_tx genesis)
{
    address_tx current = genesis;
    while (current != NULL)
    {
        bool is_valid = (strcmp(current->current_hash_block_str, current->original_hash_block_str) == 0);
        printf("%sBlock #%02d%s", is_valid ? GREEN : RED, current->id_block_tx, WHITE);
        if (current->next_block != NULL)
            printf(" -> ");
        current = current->next_block;
    }
    printf("\n\n");
}

void print_block_tx_data(address_tx block)
{
    bool is_valid = (strcmp(block->current_hash_block_str, block->original_hash_block_str) == 0);
    const char *color = is_valid ? CYAN : RED;
    printf("%s", color);
    printf("=========================================================================\n");
    printf("| BLOCK ID: #%-4d                                                        |\n", block->id_block_tx);
    printf("| NONCE: %-6d                                                        |\n", block->nonce_block_tx);
    printf("|-----------------------------------------------------------------------|\n");
    printf("| DATA TX: (%d/%d)                                                        |\n", block->transaction_count, MAX_TRANSACTIONS_IN_BLOCK);
    printf("| NO. | TX ID | FROM (ID) NAME      | TO (ID) NAME        | AMOUNT      |\n");
    printf("|-----------------------------------------------------------------------|\n");
    for (int i = 0; i < block->transaction_count; i++)
    {
        tx t = block->current_transactions[i];
        const char *tx_color = t.is_edited ? RED : color;
        printf("|%s %-3d %s|%s %-5d %s|%s %-4d %-12s %s|%s %-4d %-12s %s|%s %-10u$ %s|\n",
               tx_color, t.num, color, tx_color, t.transfer_id, color, tx_color, t.sender_id, t.sender_name, color,
               tx_color, t.receiver_id, t.receiver_name, color, tx_color, t.transfer_amount, color);
    }
    if (block->transaction_count == 0)
    {
        printf("|                 -- No transactions in this block --                 |\n");
    }
    printf("|-----------------------------------------------------------------------|\n");
    printf("| PREV HASH:     %s                                            |\n", block->prev_hash_block_str);
    printf("| MERKLE ROOT:   %s                                            |\n", block->merkle_root ? block->merkle_root->hash : "N/A_EMPTY_TX");
    printf("| CURRENT HASH:  %s                                            |\n", block->current_hash_block_str);
    printf("| ORIGINAL HASH: %s                                            |\n", block->original_hash_block_str);
    printf("=========================================================================%s\n\n", WHITE);
}

void insert_new_tx_block(address_tx *genesis)
{
    address_tx new_block = (address_tx)malloc(sizeof(block_tx_node));
    if (!new_block)
    {
        printf("Alokasi gagal!\n");
        return;
    }
    new_block->transaction_count = 0;
    new_block->merkle_root = NULL;
    new_block->next_block = NULL;
    if (*genesis == NULL)
    {
        *genesis = new_block;
        new_block->prev_block = NULL;
        new_block->id_block_tx = 1;
        strcpy(new_block->prev_hash_block_str, "000000000000");
    }
    else
    {
        address_tx last = *genesis;
        while (last->next_block != NULL)
            last = last->next_block;
        last->next_block = new_block;
        new_block->prev_block = last;
        new_block->id_block_tx = last->id_block_tx + 1;
        strcpy(new_block->prev_hash_block_str, last->original_hash_block_str);
    }
    printf("\n--- Creating New Block #%d ---\n", new_block->id_block_tx);
    printf("Pindahkan transaksi dari Mempool ke block baru ini.\n");
    mempool_menu(new_block);
    printf("\nBlock baru #%d telah dibuat. Mining block...\n", new_block->id_block_tx);
    mining_hash_for_block(new_block);
}

void validate_entire_tx_chain(address_tx genesis)
{
    if (genesis == NULL)
        return;
    strcpy(genesis->prev_hash_block_str, "000000000000");
    calculate_tx_block_hash(genesis);
    address_tx prev = genesis;
    address_tx current = genesis->next_block;
    while (current != NULL)
    {
        strcpy(current->prev_hash_block_str, prev->current_hash_block_str);
        calculate_tx_block_hash(current);
        prev = current;
        current = current->next_block;
    }
}

void transaction_submenu(address_tx block)
{
    int choice;
    do
    {
        clear_screen();
        print_block_tx_data(block);
        printf("--- Menu Transaksi Block #%d ---\n", block->id_block_tx);
        printf("1. Add New Transaction (from Mempool)\n");
        printf("2. Edit Existing Transaction\n");
        printf("3. Back\n\n");
        choice = get_integer_input("Your Option: ");
        switch (choice)
        {
        case 1:
            mempool_menu(block);
            break;
        case 2:
            handle_edit_transaction_in_block(block);
            break;
        case 3:
            break;
        default:
            printf("Pilihan tidak valid!\n");
            pause_screen();
        }
    } while (choice != 3);
}

void edit_selected_tx_block(address_tx block)
{
    int choice;
    do
    {
        clear_screen();
        calculate_tx_block_hash(block);
        print_block_tx_data(block);

        printf("%s", MAGENTA);
        printf("--- Menu Block #%d ---\n", block->id_block_tx);
        printf("1. Edit ID\n");
        printf("2. Edit Nonce\n");
        printf("3. Transaction Menu\n");
        printf("4. View Merkle Tree\n");
        printf("5. Mining (Validate This Block)\n");
        printf("6. Save All Data\n");
        printf("7. Back to Blockchain View\n\n");
        printf("%s", WHITE);

        choice = get_integer_input("Your Option: ");

        switch (choice)
        {
        case 1:
            block->id_block_tx = get_integer_input("Enter new Block ID: ");
            printf("Block ID diubah!\n");
            pause_screen();
            break;
        case 2:
            block->nonce_block_tx = get_integer_input("Enter new Nonce: ");
            printf("Nonce diubah!\n");
            pause_screen();
            break;
        case 3:
            transaction_submenu(block);
            break;
        case 4:
            view_merkle_tree_for_block(block);
            break;
        case 5:
            mining_hash_for_block(block);
            break;
        case 6:
            save_blockchain_tx_to_file(block);
            save_users_to_file();
            break;
        case 7:
            break; // Kembali
        default:
            printf("Pilihan tidak valid.\n");
            pause_screen();
        }
    } while (choice != 7);
}

address_tx find_tx_block_by_id(address_tx genesis, int id)
{
    address_tx current = genesis;
    while (current != NULL)
    {
        if (current->id_block_tx == id)
            return current;
        current = current->next_block;
    }
    return NULL;
}

void delete_tx_block_from_id(address_tx *genesis, int id)
{
    if (*genesis == NULL)
    {
        printf("Blockchain kosong!\n");
        pause_screen();
        return;
    }
    address_tx target = find_tx_block_by_id(*genesis, id);
    if (!target)
    {
        printf("Block tidak ditemukan!\n");
        pause_screen();
        return;
    }
    address_tx current_to_revert = target;
    while (current_to_revert != NULL)
    {
        for (int i = 0; i < current_to_revert->transaction_count; i++)
        {
            apply_transaction_to_database(current_to_revert->current_transactions[i], true);
        }
        current_to_revert = current_to_revert->next_block;
    }
    if (target->next_block != NULL)
    {
        free_blockchain_tx(&(target->next_block));
    }
    if (target->prev_block != NULL)
    {
        target->prev_block->next_block = NULL;
    }
    else
    {
        *genesis = NULL;
    }
    free(target);
    printf("Block #%d dan semua setelahnya berhasil dihapus. Saldo telah dikembalikan.\n", id);
    pause_screen();
}

void free_blockchain_tx(address_tx *genesis)
{
    address_tx current = *genesis, temp;
    while (current != NULL)
    {
        temp = current;
        current = current->next_block;
        if (temp->merkle_root)
            free_merkle_tree(temp->merkle_root);
        free(temp);
    }
    *genesis = NULL;
}

void save_blockchain_tx_to_file(address_tx genesis)
{
    FILE *file = fopen(BLOCK_DATA_FILE, "wb");
    if (!file)
    {
        printf("Gagal menyimpan file!\n");
        pause_screen();
        return;
    }
    address_tx current = genesis;
    while (current != NULL)
    {
        block_tx_node temp_block = *current;
        temp_block.merkle_root = NULL;
        fwrite(&temp_block, sizeof(block_tx_node), 1, file);
        current = current->next_block;
    }
    fclose(file);
    printf("Data blockchain TX berhasil disimpan!\n");
}

void load_blockchain_tx_from_file(address_tx *genesis)
{
    FILE *file = fopen(BLOCK_DATA_FILE, "rb");
    if (!file)
    {
        return;
    }
    free_blockchain_tx(genesis);
    block_tx_node file_node;
    address_tx last = NULL;
    while (fread(&file_node, sizeof(block_tx_node), 1, file) == 1)
    {
        address_tx new_block = (address_tx)malloc(sizeof(block_tx_node));
        *new_block = file_node;
        new_block->merkle_root = build_merkle_tree(new_block->current_transactions, new_block->transaction_count);
        new_block->next_block = NULL;
        new_block->prev_block = last;
        if (*genesis == NULL)
            *genesis = new_block;
        else
            last->next_block = new_block;
        last = new_block;
    }
    fclose(file);
}

char *concanate_for_tx_block(address_tx block, bool for_nonce)
{
    char *result = (char *)malloc(2048);
    strcpy(result, "");
    char temp[MAX_STRING];
    sprintf(temp, "%d", block->id_block_tx);
    strcat(result, temp);
    if (!for_nonce)
    {
        sprintf(temp, "%d", block->nonce_block_tx);
        strcat(result, temp);
    }
    if (block->merkle_root != NULL)
    {
        strcat(result, block->merkle_root->hash);
    }
    if (for_nonce)
    {
        for (int i = 0; i < block->transaction_count; i++)
        {
            tx t = block->current_transactions[i];
            sprintf(temp, "%d%s%d%s%u", t.sender_id, t.sender_name, t.receiver_id, t.receiver_name, t.transfer_amount);
            strcat(result, temp);
        }
    }
    strcat(result, block->prev_hash_block_str);
    return result;
}

void calculate_tx_block_hash(address_tx block)
{
    if (block->merkle_root)
        free_merkle_tree(block->merkle_root);
    block->merkle_root = build_merkle_tree(block->current_transactions, block->transaction_count);
    char *combined_string = concanate_for_tx_block(block, false);
    hash_generator_tx(combined_string, block->current_hash_block_str);
    free(combined_string);
}

void calculate_tx_block_nonce(address_tx block)
{
    char *combined_string = concanate_for_tx_block(block, true);
    char nonce_hash[HASH_SIZE];
    hash_generator_tx(combined_string, nonce_hash);
    long long nonce_val = 0;
    for (int i = 0; i < HASH_LEN; i++)
    {
        nonce_val += nonce_hash[i];
    }
    block->nonce_block_tx = (int)(nonce_val % 100000);
    free(combined_string);
}

void mining_hash_for_block(address_tx block)
{
    printf("Mining block hash...\n");
    calculate_tx_block_nonce(block);
    calculate_tx_block_hash(block);
    strcpy(block->original_hash_block_str, block->current_hash_block_str);
    for (int i = 0; i < block->transaction_count; i++)
    {
        block->current_transactions[i].is_edited = false;
    }
    printf("Mining success! Block is now valid.\n");
    pause_screen();
}

void handle_edit_transaction_in_block(address_tx block)
{
    clear_screen();
    print_block_tx_data(block);
    int tx_num = get_integer_input("Pilih nomor transaksi di block yang akan diedit: ");
    if (tx_num < 1 || tx_num > block->transaction_count)
    {
        printf("Nomor transaksi tidak valid!\n");
        pause_screen();
        return;
    }
    int idx = tx_num - 1;
    tx old_tx = block->current_transactions[idx];
    tx new_tx = old_tx;
    printf("Editing TX-%d\n", old_tx.transfer_id);
    new_tx.sender_id = get_integer_input("Masukkan Sender ID baru: ");
    new_tx.receiver_id = get_integer_input("Masukkan Receiver ID baru: ");
    new_tx.transfer_amount = (unsigned int)get_integer_input("Masukkan Amount baru: ");
    db_user *new_sender = find_user_by_id(new_tx.sender_id);
    db_user *new_receiver = find_user_by_id(new_tx.receiver_id);
    if (!new_sender || !new_receiver)
    {
        printf("ID sender atau receiver baru tidak valid. Perubahan dibatalkan!\n");
        pause_screen();
        return;
    }
    strcpy(new_tx.sender_name, new_sender->user_name);
    strcpy(new_tx.receiver_name, new_receiver->user_name);
    apply_transaction_to_database(old_tx, true);
    apply_transaction_to_database(new_tx, false);
    new_tx.is_edited = true;
    block->current_transactions[idx] = new_tx;
    printf("Transaksi berhasil diubah dan saldo telah disesuaikan!\n");
    pause_screen();
}

bool apply_transaction_to_database(tx transaction, bool is_reversal)
{
    db_user *sender = find_user_by_id(transaction.sender_id);
    db_user *receiver = find_user_by_id(transaction.receiver_id);
    if (sender && receiver)
    {
        if (is_reversal)
        {
            sender->user_balance += transaction.transfer_amount;
            receiver->user_balance -= transaction.transfer_amount;
        }
        else
        {
            sender->user_balance -= transaction.transfer_amount;
            receiver->user_balance += transaction.transfer_amount;
        }
        return true;
    }
    return false;
}

void view_merkle_tree_for_block(address_tx block)
{
    clear_screen();
    printf("====================================================\n");
    printf("       VISUALISASI MERKLE TREE UNTUK BLOCK #%d         \n", block->id_block_tx);
    printf("====================================================\n");
    if (block->transaction_count == 0)
    {
        printf("\nTidak ada transaksi di dalam block ini.\n");
    }
    else
    {
        if (block->merkle_root)
            free_merkle_tree(block->merkle_root);
        block->merkle_root = build_merkle_tree(block->current_transactions, block->transaction_count);
        printf("\nRoot Hash: %s\n\n", block->merkle_root ? block->merkle_root->hash : "N/A");
        char initial_prefix[10] = "";
        print_merkle_tree_visual(block->merkle_root, 0, initial_prefix);
    }
    pause_screen();
}