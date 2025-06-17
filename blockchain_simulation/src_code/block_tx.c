#include "block_tx.h"
#include "menu.h"
#include "user_management.h"
#include "queue_mempool.h"

void block_tx_simulation() {
    clear_screen();
    printf("==============================================================\n");
    printf("             SINGLE BLOCK TRANSACTION SIMULATION              \n");
    printf("==============================================================\n\n");
    
    // Inisialisasi satu block tunggal untuk simulasi ini
    block_tx_node single_block;
    single_block.id_block_tx = 1;
    single_block.nonce_block_tx = 0;
    single_block.transaction_count = 0;
    single_block.merkle_root = NULL;
    strcpy(single_block.prev_hash_block_str, "000000000000");

    // Simulasi penambahan beberapa transaksi
    printf("Menambahkan transaksi ke block...\n");
    tx tx1 = {1, 101, 21, "Fitri", 150, 33, "Zahra", "", false};
    tx tx2 = {2, 102, 1000, "Luthfi", 50, 1002, "Ardi", "", false};
    single_block.current_transactions[0] = tx1;
    single_block.current_transactions[1] = tx2;
    single_block.transaction_count = 2;
    
    // Hitung hash dan nonce untuk block ini
    calculate_tx_block_hash(&single_block);
    calculate_tx_block_nonce(&single_block);
    strcpy(single_block.original_hash_block_str, single_block.current_hash_block_str);

    print_block_tx_data(&single_block);

    printf("Ini adalah simulasi dari sebuah block transaksi tunggal.\n");
    printf("Untuk interaksi lebih lanjut seperti menambah/menghapus block (blockchain),\n");
    printf("silakan gunakan 'Blockchain TX Simulation' dari menu utama.\n");
    
    if (single_block.merkle_root) {
        free_merkle_tree(single_block.merkle_root);
    }
    pause_screen();
}