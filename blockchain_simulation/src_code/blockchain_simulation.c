/*
===================================================================================
                                    MAIN
NAMA PROGRAM: [Simulasi Blockchain]
VERSI: 1.0 [HASH FUNCTION]
CATATAN: INI ADALAH PROGRAM MAIN YANG MERANGKUM SEGALA HEADER DAN SOURCE CODE FILE
AUTHOR: [LUKMAN AHMAD | 241524050] & [MUHAMMAD LUTHFI | 241524054]
CODE LANGUAGE: ENGLISH
COMMENT LANGUAGE: ENGLISH - INDO
BAHASA PEMROGRAMAN: C 
===================================================================================
*/

#include <stdio.h>
#include "package_ADT.h"

int main()
{
    int pilihan_main_menu;
    int pilihan_sub_menu;
    
    do 
    {
        // Menu Showup
        clear_screen();
        display_main_menu();
        printf("Pilihan menu: ");
        scanf(" %d", &pilihan_main_menu);

        switch (pilihan_main_menu)
            {
            // Simulasi Hash
            case 1:
                hash_simulation();
                break;

            // Simulasi Block String
            case 2: 
                block_string_simulation();
                break;

            // Simulasi Blockchain String
            case 3: 
                blockchain_string_simulation();
                break;

            // Simulasi Block Transaksi
            case 4:
                block_tx_simulation();
                break;

            // Simulasi Blockchain Transaksi
            case 5:
                blockchain_tx_simulation();
                break;

            // Blockchain Theory
            case 6:
                blockchain_theory();
                break;

            // User Management (Implementasi CRUD)
            case 7: 
                user_management();
                break;

            // Exit 
            case 8:
                // IF SAVE FILE
                // THEN SAVEFILE()
                // EXIT(0);
                exit(0); 
                break;
            default:
                clear_screen();
                warning_error();
                pause_screen();
            }

    } while(pilihan_main_menu != 8);
    
    return 0;
}