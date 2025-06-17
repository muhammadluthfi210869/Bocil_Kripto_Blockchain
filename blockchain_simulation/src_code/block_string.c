// File C harus menyertakan header-nya sendiri untuk konsistensi
#include "block_string.h"
#include "hash_engine.h"

// Sertakan header lain yang dibutuhkan untuk implementasi
#include "menu.h"

/*
 * =================================================================================
 * File: block_string.c
 * Deskripsi: Implementasi logika untuk simulasi blok string tunggal.
 * Modul ini menunjukkan bagaimana perubahan pada data blok menginvalidasi hash-nya,
 * dan bagaimana proses "mining" (simulasi) dapat memvalidasi ulang blok tersebut.
 * =================================================================================
 */

// ======================================================================
//                       IMPLEMENTASI FUNGSI PUBLIK
// ======================================================================

// Fungsi utama yang dipanggil dari main() untuk menjalankan simulasi
void block_str_sim()
{   
    address_string genesis_block = NULL;
    int sub_menu_option = 0;

    create_block_str(&genesis_block);
    fill_block_str(&genesis_block);

    do
    {
        calculate_current_hash_string(genesis_block);
        block_str_menu(genesis_block);

        printf("Your Option: ");
        if (scanf(" %d", &sub_menu_option) != 1) 
        { // Error handling untuk input bukan angka
             sub_menu_option = 0; // Default ke invalid
        }
        while(getchar() != '\n'); 

        switch (sub_menu_option)
        {
        case 1:
            edit_id_block_str(genesis_block); 
            break;
        case 2:
            edit_nonce_block_str(genesis_block); 
            break;
        case 3:
            edit_data_block_str(genesis_block); 
            break;
        case 4:
            mining_block_str(genesis_block); 
            break;
        case 5:
            break;
        default:
            warning_error();
            pause_screen();
            break;
        }
    } while (sub_menu_option != 5);

    free(genesis_block);
    genesis_block = NULL;
}

// Menu Handling
void block_str_menu(address_string block)
{
    clear_screen();
    printf("==============================================================\n");
    printf("                  BLOCK STRING SIMULATION                     \n");
    printf("==============================================================\n\n");

    if(strcmp(block->current_hash_block_str, block->original_hash_block_str) != 0) printf("%s", RED);
    else printf("%s", GREEN);
    
    printf("|    Blok ID: %-4d                  \n", block->id_block_str);
    printf("|    Nonce: %-4d                    \n", block->nonce_block_str);
    printf("|    Data: %s                       \n", block->current_data_block_str);
    printf("|    Prev_Hash: %-36s               \n", "0 (Genesis Block)"); 
    printf("|    Current_Hash: %-12s            \n", block->current_hash_block_str);
    printf("|    Original_Hash: %-12s           \n\n", block->original_hash_block_str);
    printf("%s", WHITE);

    printf("1. Edit Blok ID\n");
    printf("2. Edit Nonce\n");
    printf("3. Edit Data\n");
    printf("4. Mining (Validate Block)\n");
    printf("5. Back\n");
}

// Operation Function
void create_block_str (address_string *node_string)
{
    *node_string = (address_string) malloc(sizeof(block_string_node));
    if (*node_string == NULL) 
    {
        printf("%sMemory allocation failed!%s", RED, WHITE);
        exit(1); 
    }
}

void fill_block_str (address_string *node_string)
{
    if (*node_string == NULL)
    {
        printf("%sError: Node not allocated%s", RED, WHITE);
        return;
    }
    
    address_string block = *node_string;

    block->id_block_str = 1;
    strcpy(block->current_data_block_str, "Hello World!");
    block->nonce_block_str = 0; // Nonce awal
    
    block->prev_block = NULL;
    block->next_block = NULL;
    
    // Hitung nonce yang valid
    block->nonce_block_str = calculate_nonce(block);

    // Hitung hash awal
    calculate_current_hash_string(block);
    
    // Salin hash awal ke original hash (Sebagai proses mining pertama)
    strcpy(block->original_hash_block_str, block->current_hash_block_str);
}

char *concanate_str (address_string block, boolean nonce_condition)
{
    char *result_concanate_str = (char *)malloc(MAX_STRING + HASH_SIZE + NONCE_LEN);
    if (result_concanate_str == NULL)
    {
        printf("Alokasi memori gagal!\n");
        return NULL;
    }
    sprintf(result_concanate_str, "%d", block->id_block_str);

    if (nonce_condition == TRUE)
    {
        char temp_nonce_str[MAX_STRING];
        sprintf(temp_nonce_str, "%d", block->nonce_block_str);
        strcat(result_concanate_str, temp_nonce_str);
    }

    strcat(result_concanate_str, block->current_data_block_str);
    strcat(result_concanate_str, block->prev_hash_block_str);
    return result_concanate_str;
}

int calculate_nonce (address_string block) // Ini untuk nonce
{
    char *combined_string = concanate_str(block, FALSE); // FALSE karena nonce belum termasuk
     if (combined_string == NULL) {
        printf("%sConcanate string failed!, Due to fail memory allocation%s\n", RED, WHITE);
        return 0;
    }

    long long digest = 0;
    int len = strlen(combined_string);

    for(int i = 0; i < len; i++) {
        digest += combined_string[i] * (i + 1);
    }
    
    free(combined_string);
    combined_string = NULL;

    // Algoritma membuat Nonce
    return (int)(digest % 100000); // Nonce berkisar 0-99999
}

void calculate_current_hash_string(address_string block) // Ini untuk hash
{
    char *combined_string = concanate_str(block, TRUE);
    if (combined_string == NULL) 
    {
        printf("%sConcanate string failed!, Due to fail memory allocation%s\n", RED, WHITE);
        return;
    }

    // Panggil hash_generator untuk menghitung hash
    hash_generator_string(combined_string, block->current_hash_block_str, FALSE);
    
    free(combined_string); // Jangan lupa free memori setelah dipakai
    combined_string = NULL;
}

void edit_id_block_str(address_string block)
{
    printf("\nInput new block ID: ");
    scanf(" %d", &block->id_block_str);
    while(getchar() != '\n'); // Membersihkan buffer

    printf("%sID Block changed succesfully%s", GREEN, WHITE);
    pause_screen();
}

void edit_nonce_block_str(address_string block)
{
    printf("\nInput new block Nonce: ");
    scanf(" %d", &block->nonce_block_str);
    while(getchar() != '\n'); // Membersihkan buffer

    printf("\n%sNonce changed succesfully%s", GREEN, WHITE);
    pause_screen();
}

void edit_data_block_str(address_string block)
{
    printf("\nInput new block data: ");
    fgets(block->current_data_block_str, sizeof(block->current_data_block_str), stdin);
    block->current_data_block_str[strcspn(block->current_data_block_str, "\n")] = 0;

    printf("\n%sData changed succesfully%s", GREEN, WHITE);
    pause_screen();
}

// Untuk simulasi cara kerjanya menjadi sangat sederhana dan tidak merepresentasikan proses aslinya
void mining_block_str(address_string block)
{
    clear_screen();
    loading_bar();

    // 1. Memperbarui nilai nonce
    block->nonce_block_str = calculate_nonce(block);
    
    // 2. Hitung ulang dengan nonce yang baru
    calculate_current_hash_string(block);

    // 3. Setelah hash valid ditemukan, salin ke original hash
    strcpy(block->original_hash_block_str, block->current_hash_block_str);

    printf("\n%sMining process done! Block is now valid.%s\n", GREEN, WHITE);
    pause_screen();
}

