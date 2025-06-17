/*
===================================================================================
                                    MAIN
NAMA PROGRAM: [Simulasi Blockchain]
VERSI: 1.4 [Blockchain Tx]
CATATAN: INI ADALAH PROGRAM MAIN YANG MERANGKUM SEGALA HEADER DAN SOURCE CODE FILE
AUTHOR: [LUKMAN AHMAD | 241524050] & [MUHAMMAD LUTHFI | 241524054]
CODE LANGUAGE: ENGLISH
COMMENT LANGUAGE: ENGLISH - INDO
BAHASA PEMROGRAMAN: C 
===================================================================================
*/
#include "package_ADT.h"

int main()
{
    srand(time(NULL));
    initialize_all();

    // Variabel declaration
    int main_menu_option = 0;
  

    do 
    {
        // Menu Showup
        display_main_menu();
        printf("Your option: ");
        scanf(" %d", &main_menu_option);
        while(getchar() != '\n'); // Clear buffer

        switch (main_menu_option)
        {
            // Simulasi Hash
            case 1:
                hash_simulation();
                break;

            // Simulasi Block String
            case 2: 
                block_str_sim();
                break;

            // Simulasi Blockchain String
            case 3: 
                blockchain_str_sim();
                break;

            // Simulasi Block Transaksi
            case 4:
                block_tx_simulation();
                break;

            // Simulasi Blockchain Transaksi
            case 5:
                blockchain_tx_sim();
                break;

            // Blockchain Theory
            case 6:
                blockchain_theory();
                break;

            // User Management (Implementasi CRUD)
            case 7: 
                exit(0); 
                break;

            default:
                clear_screen();
                warning_error();
                pause_screen();
            }

    } while(main_menu_option != 7);
    
    return 0;
}