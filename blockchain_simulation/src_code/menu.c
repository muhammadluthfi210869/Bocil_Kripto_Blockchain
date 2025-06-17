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
    printf("        Version = 1.4                                       \n");
    printf("                                                            \n");
    printf("============================================================\n");
    printf("            1. Hash Simulation                              \n");
    printf("            2. Block String Simulation                      \n");
    printf("            3. Blockchain String Simulation                 \n");
    printf("            4. Block TX Simulation                          \n");
    printf("            5. Blockchain TX Simulation                     \n");
    printf("            6. Theory About Blockchain                      \n");
    printf("            7. Exit                                         \n");
}

void warning_error()
{
    printf("%s", RED);
    printf("\nInvalid option!");
    printf("\nChose between available option!");
    printf("\n");
    printf("%s", WHITE);
}

void pause_screen() 
{
    printf("\nPress anything to continue");
    getch();
}

void loading_bar()
{
    int total_steps = 50;  // Jumlah langkah total loading
    int bar_width = 40;    // Lebar visual bar loading
    int sleep_time_ms = 50; // Waktu jeda antar langkah dalam milidetik (0.05 detik)
    const char* message = "Loading process..."; // Pesan yang tampil

    printf("%s [", message);
    fflush(stdout);

    for (int i = 0; i <= total_steps; i++) {
        printf("\r%s [", message); // Kembali ke awal baris & cetak pesan lagi

        int progress_chars = (int)(((double)i / total_steps) * bar_width);
        int percentage = (int)(((double)i / total_steps) * 100);

        for (int j = 0; j < progress_chars; j++) {
            printf("="); // Cetak bagian yang terisi
        }
        for (int j = progress_chars; j < bar_width; j++) {
            printf(" "); // Cetak bagian yang kosong
        }

        printf("] %3d%%\r", percentage); // Cetak persentase & kembali ke awal baris
        fflush(stdout);

        usleep(sleep_time_ms * 250); // Jeda. Di Windows: Sleep(sleep_time_ms);
    }
    printf("\n"); // Pindah ke baris baru setelah loading selesai
}

int get_integer_input(const char* prompt)
{
    char buffer[MAX_STRING];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%d", &value) == 1) {
                char c;
                if (sscanf(buffer, "%d %c", &value, &c) == 1) {
                    return value;
                }
            }
        }
        printf("%sInput tidak valid. Harap masukkan angka bulat.%s\n", RED, WHITE);
    }
}

long long get_long_long_input(const char* prompt)
{
    char buffer[MAX_STRING];
    long long value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%lld", &value) == 1) {
                char c;
                if (sscanf(buffer, "%lld %c", &value, &c) == 1) {
                    return value;
                }
            }
        }
        printf("%sInput tidak valid. Harap masukkan angka bulat.%s\n", RED, WHITE);
    }
}