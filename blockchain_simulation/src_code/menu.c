// File C cukup include .h dari setiap nama filenya saja
#include "menu.h" 

void clear_screen()
{
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else 
        system("clear");
    #endif
}

void display_main_menu()
{
    clear_screen();
    printf("============================================================\n");
    printf("                                                            \n");
    printf("        WELCOME TO BLOCKCHAIN SIMULATION                    \n");
    printf("        Implementation of Structure Data Algorithm          \n");
    printf("        Version = 1.0                                       \n");
    printf("                                                            \n");
    printf("============================================================\n");
    printf("            1. Hash Simulation                              \n");
    printf("            2. Block String Simulation                      \n");
    printf("            3. Blockchain String Simulation                 \n");
    printf("            4. Block TX Simulation                          \n");
    printf("            5. Blockchain TX Simulation                     \n");
    printf("            6. Theory About Blockchain                      \n");
    printf("            7. User Management                              \n");
    printf("            8. Exit                                         \n");
}

void warning_error()
{
    printf("\nPilihan Anda tidak valid");
    printf("\nPilih menu antara (1-8)!   ");
    printf("\n");
}

void pause_screen() {
    printf("\nTekan tombol apa saja untuk melanjutkan...");
    getch();
}