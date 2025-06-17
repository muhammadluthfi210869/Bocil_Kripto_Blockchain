#ifndef THEORY_H
#define THEORY_H
// Include ADT
#include "common_type.h"

/* PUBLIC FUNCTION */
void blockchain_theory();
void print_theory_menu();

void about_this_program(); // 1. This program
void about_hash(); // 2. What is hash?
void about_block(); // 3. What is block?

void about_nonce(); // 4. What is nonce?
void about_mining(); // 5. What is mining? How does this program do that?
void about_block_tx(); // 6. What does block TX do?
void about_mempool(); // 7. What does mempool do?
void about_stack_log_transaction();// 8. What does stack_log_transcation do?
void about_merkle_tree(); // 9. What does merkle tree do?


void about_blockchain(); // 10. What is blockchain?
void about_tour(); // 11. Tour all the theory features sequentially

#endif