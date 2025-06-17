// File C harus include .h dari setiap nama filenya 
#include "hash_engine.h" 

// Header tambahan
#include "menu.h"

// Fungsi utama yang dipanggil dari main() untuk menjalankan simulasi hash
void hash_simulation()
{
    printf("%s", BLUE_BG);
    int sub_menu_option = 0;
    hash_data_string simulation_data;

    // Inisialisasi data awal
    strcpy(simulation_data.current_data, "");
    hash_generator_string(simulation_data.current_data, simulation_data.original_hash, FALSE);
    
    do
    {
        clear_screen();
        hash_menu_string(&simulation_data);
        printf("Your option: ");
        scanf("%d", &sub_menu_option);
        while(getchar() != '\n'); // Clear input buffer

        switch(sub_menu_option)
        {
            case 1:
                change_data_string(&simulation_data);
                break;
            case 2:
                change_data_string(&simulation_data);
                show_process_string(&simulation_data);
                break;
            case 3:
                break; 
            default:
                warning_error();
                pause_screen();
                break;
        }
    } while (sub_menu_option != 3);
    printf("%s", BLACK_BG);
}

// Fungsi yang dipakai hanya untuk menu khusus simulasi hash
void hash_menu_string(hash_data_string *data_string)
{
    hash_generator_string(data_string->current_data, data_string->current_hash, FALSE);

    printf("==== HASH SIMULATION ====\n\n");
    printf("   Data           : %s \n", data_string->current_data);
    printf("   Current Hash   : %s \n", data_string->current_hash);
    printf("\n===================================\n");
    printf("MENU:\n");
    printf("[1] Change Data\n");
    printf("[2] Change Data & Show Process\n");
    printf("[3] Back to Main Menu\n");
}

// Fungsi untuk mengubah string menjadi hash
void hash_generator_string(const char* input_string, char* output_hash, int status_process)
{
    // Variabel Lokal
    long long digest[HASH_LEN] = {0};
    int len = strlen(input_string);

    // Print Process - 1 
    if (status_process == TRUE)
    {
        printf("\n--- STEP-1: Fill Digest ---\n");
    }

    // Mengisi nilai pada Digest
    for (int i = 0; i < len; i++)
    {
        digest[i % HASH_LEN] += input_string[i];
        if (status_process == TRUE)
        {
            printf("Char '%c' (ASCII %d) is added to digest[%d]\n", input_string[i], input_string[i], i % HASH_LEN);
        }
    }

    // Print Process - 2
    if (status_process == TRUE)
    {
        printf("\n--- STEP-2: Building Hash ---\n");
    }

    // Mengisi nilai hash dengan digest
    for (int i = 0; i < HASH_LEN; i++)
    {
        int map_val = digest[i] % 62;
        if (map_val <= 25)
        {
            output_hash[i] = (char)(map_val + 'A');
        }
        else if (map_val <= 51)
        {
            output_hash[i] = (char)((map_val - 26) + 'a');
        }
        else 
        {
            output_hash[i] = (char)((map_val - 52) + '0');
        }
        if (status_process == TRUE)
        {
            printf("Step %d : digest[%d] (ASCII %lld) MOD 62 = %d -> Result '%c'\n", 
                            i+1,        i,   digest[i],     map_val,   output_hash[i]);
            printf(" -> Hash : [");
            for (int j = 0; j <= i; j++)
            {
                printf("%c", output_hash[j]);
            }
            printf("]\n");
        }
    }

    // Null terminator
    output_hash[HASH_LEN] = '\0';
} 

// Fungsi untuk di Bin Merkle Tree & Block TX
void hash_generator_tx(const char *input, char *output) {
    long long digest[HASH_LEN] = {0};
    int len = strlen(input);
    for (int i = 0; i < len; i++) {
        digest[i % HASH_LEN] += input[i] * (i + 1);
    }
    for (int i = 0; i < HASH_LEN; i++) {
        int map_val = digest[i] % 62;
        if (map_val < 26) output[i] = (char)(map_val + 'A');
        else if (map_val < 52) output[i] = (char)((map_val - 26) + 'a');
        else output[i] = (char)((map_val - 52) + '0');
    }
    output[HASH_LEN] = '\0';
}

// Fungsi untuk mengubah data
void change_data_string(hash_data_string *data_string)
{
    printf("Enter new data: ");
    fgets(data_string->current_data, sizeof(data_string->current_data), stdin);
    data_string->current_data[strcspn(data_string->current_data, "\n")] = 0;
}

// Fungsi untuk menampilkan proses hashing
void show_process_string(hash_data_string *data_string)
{
    hash_generator_string(data_string->current_data, data_string->current_hash, TRUE);
    pause_screen();
}