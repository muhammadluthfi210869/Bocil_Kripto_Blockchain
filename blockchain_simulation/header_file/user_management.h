#ifndef USER_MANAGEMENT_H
#define USER_MANAGEMENT_H

#include "common_type.h"
#include "blockchain_tx.h" // Perlu address_tx untuk statistik

typedef struct {
    int user_id;
    char user_name[MAX_NAME];
    long long user_balance;
    bool is_active;
} db_user;

extern db_user user_database[MAX_USERS];
extern int user_count;

// Public Function
void user_management_menu(address_tx genesis);

// Operation
void initialize_all();
void create_user();
void print_user_table(db_user users_to_print[], int count);
void handle_edit_user_menu();
void find_user_menu();
void sort_users_menu(int (*compare_func)(const void*, const void*), const char* sort_type);
void get_transfer_stats_from_chain(address_tx genesis, bool find_lowest);
void save_users_to_file();
void load_users_from_file();
db_user* find_user_by_id(int id);
int compare_name_asc(const void* a, const void* b);
int compare_id_asc(const void* a, const void* b);
int compare_balance_desc(const void* a, const void* b);

#endif