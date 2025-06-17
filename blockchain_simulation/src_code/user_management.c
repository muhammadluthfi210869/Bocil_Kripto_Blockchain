#include "user_management.h"
#include "menu.h"

db_user user_database[MAX_USERS];
int user_count = 0;

void initialize_all()
{
    load_users_from_file();
    if (user_count == 0)
    {
        printf("File '%s' tidak ditemukan. Membuat data pengguna awal...\n", USER_DATA_FILE);
        user_database[0] = (db_user){56, "Luthfi", 2000, true};
        user_database[1] = (db_user){50, "Lukman", 100, true};
        user_database[2] = (db_user){85, "Ardi", 9000, true};
        user_database[3] = (db_user){21, "Fitri", 500, true};
        user_database[4] = (db_user){33, "Zahra", 750, true};
        user_database[5] = (db_user){46, "Belva", 750, true};
        user_database[6] = (db_user){44, "Dayan", 750, true};
        user_database[7] = (db_user){45, "Ihsan", 750, true};
        user_database[8] = (db_user){26, "Baba", 750, true};
        user_database[9] = (db_user){58, "Salman", 750, true};
        user_database[10] = (db_user){63, "Hana", 750, true};
        user_count = 11;
        pause_screen();
    }
}

void user_management_menu(address_tx genesis)
{
    int choice;
    do
    {
        clear_screen();
        printf("=======================================\n");
        printf("             USER MANAGEMENT           \n");
        printf("=======================================\n");
        print_user_table(user_database, user_count);
        printf("\nMenu:\n");
        printf(" 1.  Create User\n");
        printf(" 2.  Edit User\n");
        printf(" 3.  Import Users from File\n");
        printf(" 4.  Find User\n");
        printf(" 5.  Sort DB by ID (Permanent)\n");
        printf(" 6.  Sort DB by Name (Permanent)\n");
        printf(" 7.  Sort DB by Balance (Permanent)\n");
        printf(" 8.  Get Lowest Transfer in Chain\n");
        printf(" 9.  Get Highest Transfer in Chain\n");
        printf(" 10. Save All Data\n");
        printf(" 11. Back to Main Menu\n\n");

        choice = get_integer_input("Your Option: ");

        switch (choice)
        {
        case 1:
            create_user();
            break;
        case 2:
            handle_edit_user_menu();
            break;
        case 3:
            load_users_from_file();
            pause_screen();
            break;
        case 4:
            find_user_menu();
            break;
        case 5:
            sort_users_menu(compare_id_asc, "ID");
            break;
        case 6:
            sort_users_menu(compare_name_asc, "Name");
            break;
        case 7:
            sort_users_menu(compare_balance_desc, "Balance");
            break;
        case 8:
            get_transfer_stats_from_chain(genesis, true);
            break;
        case 9:
            get_transfer_stats_from_chain(genesis, false);
            break;
        case 10:
            save_users_to_file();
            save_blockchain_tx_to_file(genesis);
            break;
        case 11:
            break;
        default:
            printf("Pilihan tidak valid!\n");
            pause_screen();
        }
    } while (choice != 11);
}

void print_user_table(db_user users_to_print[], int count)
{
    printf("-----------------------------------------\n");
    printf("| No. | ID   | Name            | Balance\n");
    printf("-----------------------------------------\n");
    if (count == 0)
    {
        printf("|          No users in database         |\n");
    }
    else
    {
        int active_user_count = 0;
        for (int i = 0; i < count; i++)
        {
            if (users_to_print[i].is_active)
            {
                const char *balance_color = users_to_print[i].user_balance < 0 ? RED : WHITE;
                printf("| %-3d | %-4d | %-15s | %s%-10lld$%s\n", active_user_count + 1, users_to_print[i].user_id, users_to_print[i].user_name, balance_color, users_to_print[i].user_balance, WHITE);
                active_user_count++;
            }
        }
    }
    printf("-----------------------------------------\n");
    printf("Total Users: %d\n", user_count);
}

db_user *find_user_by_id(int id)
{
    for (int i = 0; i < user_count; i++)
    {
        if (user_database[i].user_id == id && user_database[i].is_active)
        {
            return &user_database[i];
        }
    }
    return NULL;
}

void create_user()
{
    if (user_count >= MAX_USERS)
    {
        printf("Database pengguna sudah penuh!\n");
        pause_screen();
        return;
    }
    db_user new_user;
    new_user.is_active = true;
    new_user.user_id = get_integer_input("Enter new User ID: ");
    if (find_user_by_id(new_user.user_id) != NULL)
    {
        printf("Error: User ID %d sudah ada!\n", new_user.user_id);
        pause_screen();
        return;
    }
    printf("Enter new User Name: ");
    fgets(new_user.user_name, MAX_NAME, stdin);
    new_user.user_name[strcspn(new_user.user_name, "\n")] = 0;
    new_user.user_balance = get_long_long_input("Enter initial Balance: ");
    user_database[user_count++] = new_user;
    printf("User baru berhasil ditambahkan!\n");
    pause_screen();
}

void find_user_menu()
{
    int id_to_find = get_integer_input("Enter User ID to find: ");
    db_user *user = find_user_by_id(id_to_find);
    if (user)
    {
        printf("\nUser Ditemukan!\nID: %d, Name: %s, Balance: %lld$\n", user->user_id, user->user_name, user->user_balance);
    }
    else
    {
        printf("\nUser dengan ID %d tidak ditemukan.\n", id_to_find);
    }
    pause_screen();
}

void handle_edit_user_menu()
{
    print_user_table(user_database, user_count);
    int user_num = get_integer_input("Pilih nomor user yang ingin diubah: ");
    if (user_num < 1 || user_num > user_count)
    {
        printf("Nomor user tidak valid!\n");
        pause_screen();
        return;
    }
    int idx = user_num - 1;
    int choice;
    do
    {
        clear_screen();
        printf("Editing User: %s (ID: %d, Balance: %lld$)\n\n", user_database[idx].user_name, user_database[idx].user_id, user_database[idx].user_balance);
        printf("1. Edit Name\n2. Top-Up Balance\n3. Back\n\n");
        choice = get_integer_input("Your Option: ");
        if (choice == 1)
        {
            printf("Enter new name: ");
            fgets(user_database[idx].user_name, MAX_NAME, stdin);
            user_database[idx].user_name[strcspn(user_database[idx].user_name, "\n")] = 0;
            printf("Nama berhasil diubah!\n");
            pause_screen();
        }
        else if (choice == 2)
        {
            long long amount = get_long_long_input("Enter amount to top-up: ");
            if (amount > 0)
            {
                user_database[idx].user_balance += amount;
                printf("Balance berhasil ditambah!\n");
            }
            else
            {
                printf("Jumlah top-up harus positif!\n");
            }
            pause_screen();
        }
    } while (choice != 3);
}

int compare_name_asc(const void *a, const void *b) { return strcmp(((db_user *)a)->user_name, ((db_user *)b)->user_name); }
int compare_balance_desc(const void *a, const void *b)
{
    if (((db_user *)a)->user_balance > ((db_user *)b)->user_balance)
        return -1;
    if (((db_user *)a)->user_balance < ((db_user *)b)->user_balance)
        return 1;
    return 0;
}
int compare_id_asc(const void *a, const void *b)
{
    if (((db_user *)a)->user_id < ((db_user *)b)->user_id)
        return -1;
    if (((db_user *)a)->user_id > ((db_user *)b)->user_id)
        return 1;
    return 0;
}

void sort_users_menu(int (*compare_func)(const void *, const void *), const char *sort_type)
{
    qsort(user_database, user_count, sizeof(db_user), compare_func);
    printf("Database telah diurutkan secara permanen berdasarkan %s.\n", sort_type);
    pause_screen();
}

void get_transfer_stats_from_chain(address_tx genesis, bool find_lowest)
{
    if (genesis == NULL)
    {
        printf("\nBlockchain kosong, tidak ada transaksi untuk dianalisis.\n");
        pause_screen();
        return;
    }
    unsigned int result_amount = find_lowest ? -1 : 0;
    bool tx_found = false;
    address_tx current_block = genesis;
    while (current_block != NULL)
    {
        for (int i = 0; i < current_block->transaction_count; i++)
        {
            tx_found = true;
            unsigned int current_amount = current_block->current_transactions[i].transfer_amount;
            if (find_lowest)
            {
                if (current_amount < result_amount)
                    result_amount = current_amount;
            }
            else
            {
                if (current_amount > result_amount)
                    result_amount = current_amount;
            }
        }
        current_block = current_block->next_block;
    }

    if (!tx_found)
    {
        printf("\nTidak ada transaksi di dalam blockchain untuk dianalisis.\n");
        pause_screen();
        return;
    }
    printf("\nTransaksi %s di seluruh Rantai Block: %u$\n", find_lowest ? "Terendah" : "Tertinggi", result_amount);
    pause_screen();
}

void save_users_to_file()
{
    FILE *file = fopen(USER_DATA_FILE, "wb");
    if (!file)
    {
        printf("Gagal membuka file untuk menyimpan!\n");
        pause_screen();
        return;
    }
    fwrite(&user_count, sizeof(int), 1, file);
    fwrite(user_database, sizeof(db_user), user_count, file);
    fclose(file);
    printf("Data pengguna berhasil disimpan ke '%s'\n", USER_DATA_FILE);
}

void load_users_from_file()
{
    FILE *file = fopen(USER_DATA_FILE, "rb");
    if (!file)
    {
        return;
    }
    fread(&user_count, sizeof(int), 1, file);
    if (user_count > 0 && user_count <= MAX_USERS)
        fread(user_database, sizeof(db_user), user_count, file);
    else
        user_count = 0;
    fclose(file);
    printf("Data pengguna berhasil dimuat dari '%s'\n", USER_DATA_FILE);
}