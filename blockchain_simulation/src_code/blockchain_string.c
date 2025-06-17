#include "blockchain_string.h"

#include "menu.h"

// Public Function (bisa dipanggil di main)
void blockchain_str_sim()
{
    address_string genesis_block = NULL;
    int sub_menu_option = 0;
    load_blockchain_from_file(&genesis_block);
    do
    {
        clear_screen();
        validate_entire_chain(genesis_block);
        blockchain_str_menu(genesis_block);
        printf("Your Option: ");
        if (scanf(" %d", &sub_menu_option) != 1)
        {
            sub_menu_option = 0;
        }
        while (getchar() != '\n');
        switch (sub_menu_option)
        {
        case 1:
            printf("Enter Block ID to view/edit: ");
            int block_id_choice;
            if (scanf(" %d", &block_id_choice) == 1)
            {
                address_string selected_block = find_block_by_id(genesis_block, block_id_choice);
                if (selected_block != NULL)
                {
                    edit_selected_block(selected_block);
                }
                else
                {
                    printf("%sBlock with ID %d not found!%s\n", RED, block_id_choice, WHITE);
                    pause_screen();
                }
            }
            while (getchar() != '\n')
                ;
            break;
        case 2:
            insert_new_block(&genesis_block);
            break;
        case 3:
            delete_block_from_id(&genesis_block);
            break;
        case 4:
            save_blockchain_to_file(genesis_block);
            break;
        case 5:
            break;
        default:
            warning_error();
            pause_screen();
            break;
        }
    } while (sub_menu_option != 5);
    free_blockchain(&genesis_block);
}

void blockchain_str_menu(address_string genesis)
{
    printf("==============================================================\n");
    printf("                  BLOCKCHAIN STRING SIMULATION                  \n");
    printf("==============================================================\n\n");
    if (genesis == NULL)
    {
        printf("%sBlock Kosong!!!%s\n\n", YELLOW, WHITE);
    }
    else
    {
        print_blockchain_status(genesis);
    }
    printf("1. View/Edit Block by ID\n");
    printf("2. Insert New Block\n");
    printf("3. Delete Block by ID\n");
    printf("4. Save Blockchain to File\n");
    printf("5. Back (Exit Program)\n\n");
}

void insert_new_block(address_string *genesis)
{
    address_string new_block = (address_string)malloc(sizeof(block_string_node));
    if (new_block == NULL)
    {
        printf("Alokasi memori gagal!\n");
        return;
    }
    printf("\nEnter data for the new block: ");
    fgets(new_block->current_data_block_str, MAX_STRING, stdin);
    new_block->current_data_block_str[strcspn(new_block->current_data_block_str, "\n")] = 0;
    new_block->next_block = NULL;
    if (*genesis == NULL)
    {
        *genesis = new_block;
        new_block->prev_block = NULL;
        new_block->id_block_str = 1;
        strcpy(new_block->prev_hash_block_str, "000000000000");
    }
    else
    {
        address_string last_block = *genesis;
        while (last_block->next_block != NULL)
        {
            last_block = last_block->next_block;
        }
        last_block->next_block = new_block;
        new_block->prev_block = last_block;
        new_block->id_block_str = last_block->id_block_str + 1;
        strcpy(new_block->prev_hash_block_str, last_block->original_hash_block_str);
    }
    mining_block_str(new_block);
    printf("\nNew block #%d successfully inserted and mined!\n", new_block->id_block_str);
    pause_screen();
}

void delete_block_from_id(address_string *genesis)
{
    if (*genesis == NULL)
    {
        printf("%sBlockchain is already empty!%s\n", YELLOW, WHITE);
        pause_screen();
        return;
    }
    int id_to_delete;
    printf("Enter Block ID to delete (this will delete the block and all subsequent blocks): ");
    if (scanf(" %d", &id_to_delete) != 1)
    {
        while (getchar() != '\n')
            ;
        printf("%sInvalid input!%s\n", RED, WHITE);
        pause_screen();
        return;
    }
    while (getchar() != '\n');
    address_string block_to_delete = find_block_by_id(*genesis, id_to_delete);
    if (block_to_delete == NULL)
    {
        printf("%sBlock with ID %d not found!%s\n", RED, id_to_delete, WHITE);
        pause_screen();
        return;
    }
    free_blockchain(&(block_to_delete->next_block));
    if (block_to_delete->prev_block != NULL)
    {
        block_to_delete->prev_block->next_block = NULL;
    }
    else
    {
        *genesis = NULL;
    }
    free(block_to_delete);
    printf("\n%sBlock #%d and all subsequent blocks have been deleted.%s\n", GREEN, id_to_delete, WHITE);
    pause_screen();
}

void edit_selected_block(address_string block)
{
    int sub_menu_option = 0;
    do
    {
        calculate_current_hash_string(block);
        edit_selected_block_menu(block);
        printf("Your Option: ");
        if (scanf(" %d", &sub_menu_option) != 1)
        {
            sub_menu_option = 0;
        }
        while (getchar() != '\n')
            ;
        switch (sub_menu_option)
        {
        case 1: // Edit ID
            printf("\nInput new block ID: ");
            scanf("%d", &block->id_block_str);
            while (getchar() != '\n')
                ;
            printf("\nID block changed!\n");
            pause_screen();
            break;
        case 2: // Edit Nonce
            printf("\nInput new block Nonce: ");
            scanf("%d", &block->nonce_block_str);
            while (getchar() != '\n')
                ;
            printf("\nNonce changed!\n");
            pause_screen();
            break;
        case 3: // Edit Data
            printf("\nInput new block data: ");
            fgets(block->current_data_block_str, MAX_STRING, stdin);
            block->current_data_block_str[strcspn(block->current_data_block_str, "\n")] = 0;
            printf("\nData changed!\n");
            pause_screen();
            break;
        case 4: // Mining
            mining_block_str(block);
            break;
        case 5: // Back
            break;
        default:
            warning_error();
            pause_screen();
            break;
        }
    } while (sub_menu_option != 5);
}

void edit_selected_block_menu(address_string block)
{
    clear_screen();
    printf("==============================================================\n");
    printf("                      VIEW/EDIT BLOCK #%02d                     \n", block->id_block_str);
    printf("==============================================================\n\n");
    if (strcmp(block->current_hash_block_str, block->original_hash_block_str) != 0)
    {
        printf("%s", RED);
    }
    else
    {
        printf("%s", GREEN);
    }
    printf("| Block ID        : %-d\n", block->id_block_str);
    printf("| Nonce           : %-d\n", block->nonce_block_str);
    printf("| Data            : %-s\n", block->current_data_block_str);
    printf("| Prev_Hash       : %-s\n", block->prev_block ? block->prev_hash_block_str : "000000000000 (Genesis)");
    printf("| Current_Hash    : %-s\n", block->current_hash_block_str);
    printf("| Original_Hash   : %-s\n\n", block->original_hash_block_str);
    printf("%s", WHITE);
    printf("1. Edit ID\n");
    printf("2. Edit Nonce\n");
    printf("3. Edit Data\n");
    printf("4. Mining (Validate This Block)\n");
    printf("5. Back to Blockchain View\n\n");
}

void validate_entire_chain(address_string genesis)
{
    if (genesis == NULL) return;

    // Step 1: Hitung ulang hash Genesis, karena dia tidak punya predecessor.
    // Pastikan prev_hash-nya adalah "0" dan current_hash-nya sesuai datanya.
    strcpy(genesis->prev_hash_block_str, "000000000000");
    calculate_current_hash_string(genesis);

    // Step 2: Iterasi dari block kedua dan seterusnya
    address_string previous = genesis;
    address_string current = genesis->next_block;

    while (current != NULL)
    {
        // Update prev_hash di block 'current' dengan *current_hash* dari block 'previous'.
        // Inilah kunci dari efek berantainya.
        strcpy(current->prev_hash_block_str, previous->current_hash_block_str);

        // Hitung ulang hash 'current' berdasarkan data barunya (termasuk prev_hash yang mungkin baru).
        calculate_current_hash_string(current);

        // Lanjut ke block berikutnya
        previous = current;
        current = current->next_block;
    }
}

void print_blockchain_status(address_string genesis)
{
    address_string current = genesis;
    while (current != NULL)
    {
        if (strcmp(current->current_hash_block_str, current->original_hash_block_str) != 0) printf("%s", RED); 
        else printf("%s", GREEN); 

        printf("Block #%02d", current->id_block_str);
        printf("%s", WHITE);

        if (current->next_block != NULL) printf(" -> "); 
        current = current->next_block;
    }
    printf("\n\n");
}

address_string find_block_by_id(address_string genesis, int id)
{
    address_string current = genesis;
    while (current != NULL)
    {
        if (current->id_block_str == id)
        {
            return current;
        }
        current = current->next_block;
    }
    return NULL;
}

void free_blockchain(address_string *genesis)
{
    address_string current = *genesis, temp_node;
    while (current != NULL)
    {
        temp_node = current;
        current = current->next_block;
        free(temp_node);
    }
    *genesis = NULL;
}

void save_blockchain_to_file(address_string genesis)
{
    FILE *file = fopen(FILENAME_STR, "wb");
    if (file == NULL)
    {
        printf("%sError opening file!%s\n", RED, WHITE);
        pause_screen();
        return;
    }
    address_string current = genesis;
    while (current != NULL)
    {
        fwrite(current, sizeof(block_string_node), 1, file);
        current = current->next_block;
    }
    fclose(file);
    printf("\n%sBlockchain data has been saved successfully to %s%s\n", GREEN, FILENAME_STR, WHITE);
    pause_screen();
}

void load_blockchain_from_file(address_string *genesis)
{
    FILE *file = fopen(FILENAME_STR, "rb");
    if (file == NULL)
    {
        create_initial_chain(genesis);
        return;
    }
    free_blockchain(genesis);
    block_string_node file_node;
    address_string last_block = NULL;
    while (fread(&file_node, sizeof(block_string_node), 1, file) == 1)
    {
        address_string new_block = (address_string)malloc(sizeof(block_string_node));
        if (new_block == NULL)
        {
            printf("Memory allocation failed!\n");
            fclose(file);
            return;
        }
        *new_block = file_node; // Salin semua data
        new_block->next_block = NULL;
        new_block->prev_block = last_block;
        if (*genesis == NULL)
        {
            *genesis = new_block;
        }
        else
        {
            last_block->next_block = new_block;
        }
        last_block = new_block;
    }
    fclose(file);
}

void create_initial_chain(address_string *genesis)
{
    printf("Save file not found. Creating a new default blockchain...\n");
    // Gunakan fungsi insert agar lebih konsisten
    address_string temp_genesis = NULL;
    insert_new_block(&temp_genesis); // Block 1
    strcpy(temp_genesis->current_data_block_str, "Blok Pertama");
    mining_block_str(temp_genesis);

    insert_new_block(&temp_genesis); // Block 2
    strcpy(temp_genesis->next_block->current_data_block_str, "Data Blok Kedua");
    mining_block_str(temp_genesis->next_block);

    insert_new_block(&temp_genesis); // Block 3
    strcpy(temp_genesis->next_block->next_block->current_data_block_str, "Ini Blok Terakhir");
    mining_block_str(temp_genesis->next_block->next_block);
    
    *genesis = temp_genesis;
    clear_screen();
    printf("Initial chain with 3 blocks has been created.\n");
    pause_screen();
}