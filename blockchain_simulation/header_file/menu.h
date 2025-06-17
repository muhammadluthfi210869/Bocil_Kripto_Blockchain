#ifndef MENU_H
#define MENU_H

// Include 
#include "common_type.h"

// FUNCTION
void clear_screen();
void display_main_menu();
void warning_error();
void pause_screen();
void loading_bar();
int get_integer_input(const char* prompt);
long long get_long_long_input(const char* prompt);

#endif