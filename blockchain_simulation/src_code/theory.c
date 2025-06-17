#include "theory.h"
#include "menu.h"

void blockchain_theory()
{
    int sub_menu_option = 0;
    do 
    {
        print_theory_menu();
        printf("Your Option: ");

        if (scanf(" %d", &sub_menu_option) != 1) 
        { // Error handling untuk input bukan angka
             sub_menu_option = 0; // Default ke invalid
        }

        switch (sub_menu_option)
        {
        // About this program
        case 1:
            about_this_program();
            break;  

        // About Hash
        case 2:
            about_hash();
            break;
        
        // About Block
        case 3:
            about_block();
            break;
        
        // About Nonce
        case 4:
            about_nonce();
            break;
        
        // About Mining
        case 5:
            about_mining();
            break;

        // About Block_TX
        case 6:
            about_block_tx();
            break;
        
        // About Mempool
        case 7:
            about_mempool();
            break;
        
        // About Stack Log Transaction
        case 8:
            about_stack_log_transaction();
            break;

        // About Merkle Tree
        case 9:
            about_merkle_tree();
            break;     

        // About Blockchain
        case 10:
            about_blockchain();
            break;

        // Tour Theory
        case 11:
            about_tour();
            break;

        // EXIT
        case 12: 
            printf("Back loading...");
            usleep(200000);
            break;
        
        // Invalid option 
        default:
            warning_error();
            pause_screen();
            break;
        }

    } while (sub_menu_option != 12);

}

void print_theory_menu()
{
    clear_screen();
    printf("%s==========================================================\n", GREEN);
    printf("                                                          \n");
    printf("                      BLOCKCHAIN THEORY                   \n");
    printf("                  (LET'S HAVE SOME TOUR!)                 \n");
    printf("                                                          \n");
    printf("==========================================================%s\n", WHITE);
    printf("1.  About this program           \n");
    printf("2.  About Hash                   \n");
    printf("3.  About Block                  \n");
    printf("4.  About Nonce                  \n");
    printf("5.  About Mining                 \n");
    printf("6.  About Block Tx               \n");
    printf("7.  About Mempool                \n");
    printf("8.  About Stack Log Transaction  \n");
    printf("9.  About Merkle Tree            \n");
    printf("10. About Blockchain             \n");
    printf("11. Tour (Try all theory in one) \n");
    printf("12. Back to main menu            \n");
}

void about_this_program() {
    clear_screen();
    printf("%s====================================================\n", GREEN);
    printf("                BLOCKCHAIN SIMULATION               \n");
    printf("====================================================%s\n", WHITE);
    printf("\n");
    printf("This program was made for educational purposes. We try to\n");
    printf("implement data structure algorithms for some blockchain features.\n");
    printf("This program doesn't 100%% implement real-life blockchain,\n");
    printf("and some features have minimized scope.\n\n");
    printf("However, we hope you can understand the basic principles\n");
    printf("of blockchain technology through this simulation.\n\n");
    printf("%sFeatures included:%s\n", GREEN, WHITE);
    printf("- Hash function simulation\n");
    printf("- Block structure\n");
    printf("- Mining process with nonce\n");
    printf("- Transaction handling\n");
    printf("- Mempool management\n");
    printf("- Merkle tree implementation\n");
    printf("- Basic blockchain operations\n\n");

    pause_screen();
}

void about_hash()
{
    clear_screen();
    printf("%s====================================================\n", GREEN);
    printf("                    ABOUT HASH                      \n");
    printf("====================================================%s\n", WHITE);
    printf("\n");
    printf("Hash is a mathematical function that converts input data\n");
    printf("of any size into a fixed-size string of characters.\n\n");

    printf("%sKey Properties:%s\n", GREEN, WHITE);
    printf("- Deterministic: Same input always produces same output\n");
    printf("- Fast computation: Quick to calculate\n");
    printf("- Avalanche effect: Small input change = Big output change\n");
    printf("- One-way function: Hard to reverse\n");
    printf("- Collision resistant: Hard to find two inputs with same hash\n\n");

    printf("%sHow it works on our programs:%s\n", GREEN, WHITE);
    printf("This program not actually implementing the SHA-256 Hash.\n");
    printf("So, we made our own Algorithm that can implement hash in C\n");

    printf("ALGORITHM hash_generator_string\n");
    printf("INPUT:\n");
    printf("    - input_string: string to be hashed\n");
    printf("    - output_hash: array to store the resulting hash\n");
    printf("OUTPUT:\n");
    printf("    - output_hash: filled with generated hash characters\n");

    printf("BEGIN\n");
    printf("    DECLARE digest[HASH_LEN] as array of long long integers, initialize to 0\n");
    printf("    SET len = length of input_string\n");

    printf("    // STEP 1: Fill Digest Array\n");
    printf("    FOR i = 0 TO len-1 DO\n");
    printf("        SET digest[i MOD HASH_LEN] = digest[i MOD HASH_LEN] + ASCII_value(input_string[i])\n");
    printf("    END FOR\n");

    printf("    // STEP 2: Build Hash from Digest\n");
    printf("    FOR i = 0 TO HASH_LEN-1 DO\n");
    printf("        SET map_val = digest[i] MOD 62\n\n");

    printf("        IF map_val <= 25 THEN\n");
    printf("            SET output_hash[i] = character(map_val + ASCII_value('A'))\n");
    printf("        ELSE IF map_val <= 51 THEN\n");
    printf("            SET output_hash[i] = character((map_val - 26) + ASCII_value('a'))\n");
    printf("        ELSE\n");
    printf("            SET output_hash[i] = character((map_val - 52) + ASCII_value('0'))\n");
    printf("        END IF\n");
    printf("    END FOR\n");
    printf("    SET output_hash[HASH_LEN] = '\\0'\n");
    printf("END\n");

    pause_screen();
}

void about_block()
{
    clear_screen();
    printf("%s====================================================\n", GREEN);
    printf("                    ABOUT BLOCK                       \n");
    printf("====================================================%s\n", WHITE);
    
    printf("Block is a fundamental unit of a blockchain. Each block contains\n");
    printf("a collection of data or transactions, which have unique identifier\n");
    printf("called a nonce, reference to the previous block (prev_hash)\n");
    printf("and it's own cryptographic (hash_value). This structure made\n");
    printf("the immutability and integrity of the blockchain\n\n");
    
    printf("====================================================\n");
    printf("|                                                  |\n");
    printf("|  BLOCK ID = #01                                  |\n");
    printf("|  NONCE = 412345                                  |\n");
    printf("|  DATA = \"Hello World\"                            |\n");
    printf("|                                                  |\n");
    printf("|  PREV_HASH = 000000000000                        |\n");
    printf("|  CURRENT HASH = 000X1228BXAVY                    |\n");
    printf("|                                                  |\n");
    printf("====================================================\n\n");

    printf("->  Block ID: is number identifier that increment each block created\n");
    printf("->  Nonce: An unique identifier that represent block data           \n");
    printf("->  Data: Content of the block, for this example is String Data\n");
    printf("->  Prev_Hash: A pointer that contains previous_hash of the block\n");
    printf("->  Current_hash: A cryptographic identifier for block\n");

    pause_screen();
}

void about_nonce()
{
    clear_screen();
    printf("%s====================================================\n", GREEN);
    printf("                    ABOUT NONCE                       \n");
    printf("====================================================%s\n", WHITE);

    printf("====================================================\n");
    printf("|                                                  |\n");
    printf("|  BLOCK ID = #01                                  |\n");
    printf("%s|  NONCE = 412345 -> THIS IS NONCE                 |\n%s", GREEN, WHITE);
    printf("|  DATA = \"Hello World\"                          |\n");
    printf("|                                                  |\n");
    printf("|  PREV_HASH = 000000000000                        |\n");
    printf("|  CURRENT HASH = 000X1228BXAVY                    |\n");
    printf("|                                                  |\n");
    printf("====================================================\n");

    printf("-> On this program, Block nonce created not represent a real blockchain features.\n");
    printf("   However, it's still functional as unique identifier and have own algorithm.\n");

    printf("Nonce Creator Algorithm");

    printf("ALGORITHM: calculate_nonce\n");
    printf("INPUT:\n");
    printf("    - block: block structure containing block data\n");
    printf("OUTPUT:\n");
    printf("    - nonce: integer value (0-99999) representing the calculated nonce\n\n");

    printf("BEGIN\n");
    printf("    // Step 1: Concatenate block data into string\n");
    printf("    SET combined_string = concatenate_block_data(block, exclude_nonce=TRUE)\n\n");

    printf("    // Step 2: Check for memory allocation failure\n");
    printf("    IF combined_string == NULL THEN\n");
    printf("        RETURN 0\n");
    printf("    END IF\n\n");

    printf("    // Step 3: Initialize digest calculation\n");
    printf("    SET digest = 0\n");
    printf("    SET len = length_of(combined_string)\n\n");

    printf("    // Step 4: Calculate weighted digest\n");
    printf("    FOR i = 0 TO len-1 DO\n");
    printf("        SET digest = digest + (ASCII_value(combined_string[i]) × (i + 1))\n");
    printf("    END FOR\n\n");

    printf("    // Step 5: Clean up memory\n");
    printf("    DEALLOCATE combined_string\n");
    printf("    SET combined_string = NULL\n\n");

    printf("    // Step 6: Generate nonce from digest\n");
    printf("    SET nonce = digest MOD 100000\n\n");

    printf("    RETURN nonce\n");
    printf("END\n");

    pause_screen();
}

void about_mining()
{
    clear_screen();
    printf("%s====================================================\n", GREEN);
    printf("                    ABOUT MINING                      \n");
    printf("====================================================%s\n", WHITE);

    printf("In this blockchain simulation, mining is the process\n");
    printf("of validating a block by generating a valid nonce.\n");
    printf("Here's how mining works in the system:\n\n");

    printf("1. First, the block data (excluding nonce) is processed\n");
    printf("   using a digest function to calculate a special number\n");
    printf("   called the 'nonce'.\n\n");

    printf("2. Then, the nonce is combined with the block data to\n");
    printf("   recalculate the current hash of the block.\n\n");

    printf("3. Once a valid hash is generated, it is considered the\n");
    printf("   official (original) hash of the block, and the block\n");
    printf("   becomes valid and immutable.\n\n");

    printf("   Visual Proc:                             \n");
    printf("   1. Data changed                          \n");
    printf("   %sPrevious Hash / Original Hash: 000190xxxx <- It's different%s\n", RED, WHITE);
    printf("   Current Hash:                  000123xxxx   <-\n\n");

    printf("   Visual Proc:                             \n");
    printf("   2. Copy value                            \n");
    printf("   %sPrevious Hash / Original Hash: 000190xxxx <- Now It's same!\n", GREEN);
    printf("   Current Hash:                  000123xxxx   <-%s\n", WHITE);


    printf("%sNote: This is a simplified version of real-world mining.\n", RED);
    printf("There's no complex cryptographic puzzle or proof-of-work\n");
    printf("difficulty, but the structure still mimics the steps.%s\n", WHITE);


    pause_screen();
}

void about_block_tx()
{
    clear_screen();
    printf("%s====================================================\n", GREEN);
    printf("               ABOUT BLOCK_TRANSACTION                \n");
    printf("====================================================%s\n", WHITE);

    printf("=====================================================================\n");
    printf("|                                                                   |\n");
    printf("|  BLOCK ID = #01                                                   |\n");
    printf("|  NONCE = 219441                                                   |\n");
    printf("|  DATA TX:                                                         |\n");
    printf("|  NO    FROM               TO                         AMOUNT       |\n");
    printf("|        ID    NAME         ID      NAME                            |\n");
    printf("|  [1]   0021  FITRI        0033    ZAHRA              100$         |\n");
    printf("|  [2]   0007  CRISTIANO    0011    NUNO MENDES        255$         |\n");
    printf("|  [3]   0083  MICHAEL      0018    NACHO              2000$        |\n");
    printf("|  [4]   0091  GEORGE       0063    DEVIN              500$         |\n");
    printf("|                                                                   |\n");
    printf("|  PREV_HASH = 000000000000                                         |\n");
    printf("|  CURRENT HASH = 00PZ190KLGF10Y                                    |\n");
    printf("|                                                                   |\n");
    printf("=====================================================================\n\n");

    printf("Same as block that mentioned before, but the data contains transaction\n");
    printf("between user. However, in our program the system have database that stores\n");
    printf("users data, that doesn't represent in real-life blockchain system (database)\n");

    printf("NOTE:                                                                        \n");
    printf("-> Every transaction was added to Mempool first before get added to the block\n");
    printf("-> Mining process make the (n) transaction in the mempool processed into data\n\n");

    printf("HASH WORK\n");
    printf("Similar to previous algorithm, here's the simplyfied process\n");
    printf("1. Concanate all data into string\n");
    printf("2. The data sent into hash function\n");
    printf("3. Update current hash\n");

    printf("MINING PROCESS FROM MEMPOOL\n");
    printf("Some transaction from mempool will added into block\n");
    printf("to fill some remaining data spaces\n");

    printf("OTHER'S WORK\n");
    printf("A process like finding nonce and mining for hash validation has same algorithm\n");

    pause_screen();
}

void about_mempool()
{
    clear_screen();
    printf("%s====================================================\n", GREEN);
    printf("                   ABOUT MEMPOOL                      \n");
    printf("====================================================%s\n\n", WHITE);

    printf("In this simulation, the mempool (memory pool) is a\n");
    printf("temporary queue that stores pending transactions before\n");
    printf("they are added to a block.\n\n");

    printf("This structure is implemented using a linear queue,\n");
    printf("where new transactions are added to the tail (ENQUEUE),\n");
    printf("and transactions to be mined are taken from the head\n");
    printf("(DEQUEUE).\n\n");

    printf("This process mimics how real-world blockchains work,\n");
    printf("although actual mempools are far more complex and\n");
    printf("involve priority, fees, timestamps, and peer-to-peer\n");
    printf("network handling.\n\n");

    printf("Our simulation focuses on the basic mechanism to help\n");
    printf("understand how transactions are temporarily held and\n");
    printf("processed in order.\n");

    printf("VISUAL: \n");
    printf("=========================================================\n");
    printf("| QUEUE                                                 |\n");
    printf("|                                                       |\n");
    printf("|   HEAD                                                |\n");
    printf("|    |                                                  |\n");
    printf("|    V                                                  |\n");
    printf("| (TX-01)-(TX-02)-(TX-03)-(TX 04)-(TX-05)-(TX-06)       |\n");
    printf("|    ^               ^                        ^         |\n");
    printf("|    |               |                        |         |\n");
    printf("| DEQUEUE          LIMIT                   ENQUEUE      |\n");
    printf("|                                                       |\n");
    printf("=========================================================\n");

    printf("MINING VISUAL PROCESS\n");
    printf("Let's say there are 3 Slot left for transaction in Block A\n\n");

    printf("1. First-Step: \n");
    printf("=========================================================\n");
    printf("| QUEUE                                                 |\n");
    printf("|                                                       |\n");
    printf("|   HEAD                                                |\n");
    printf("|    |                                                  |\n");
    printf("|    V                                                  |\n");
    printf("| %s(TX-01)-(TX-02)-(TX-03)%s-(TX 04)-(TX-05)-(TX-06)       |\n", GREEN, WHITE);
    printf("|    ^               ^                        ^         |\n");
    printf("|    |               |                        |         |\n");
    printf("| DEQUEUE          LIMIT                   ENQUEUE      |\n");
    printf("|                                                       |\n");
    printf("=========================================================\n\n");

    printf("                MINING MEMPOOL!!!                        \n\n");

    printf("2. Next-Step: \n");
    printf("=========================================================\n");
    printf("| QUEUE                                                 |\n");
    printf("|                                                       |\n");
    printf("|   HEAD                                                |\n");
    printf("|    |                                                  |\n");
    printf("|    V                                                  |\n");
    printf("| (TX 04)-(TX-05)-(TX-06)                               |\n");
    printf("|    ^               ^                                  |\n");
    printf("|    |               |                                  |\n");
    printf("| DEQUEUE         ENQUEUE                               |\n");
    printf("| %sLIMIT = 0%s                                             |\n", RED, WHITE);
    printf("=========================================================\n\n");   

    printf("                MINING SUCCESS!!!                        \n\n");

    pause_screen();
}

void about_stack_log_transaction()
{
    clear_screen();
    printf("%s====================================================\n", GREEN);
    printf("                ABOUT LOG TRANSACTION                 \n");
    printf("====================================================%s\n", WHITE);

    printf("Log transaction is implementation of stack in our programs\n");
    printf("that record transaction history inside mempool, we can Undo and Redo\n");
    printf("some transaction to mempool, but when data is mined from the mempool, \n");
    printf("then it can't be changed or do some undo/redo operation.              \n\n");

    printf("-> Every added transaction into mempool will be recorded\n");
    printf("-> Every Undo/Redo operation will affect to mempool\n");

    printf("Visual\n");
    printf("1. Data TX-6 was deleted from Mempool!\n");
    printf("2. Data TX-6 was added to Mempool!\n");
    printf("3. Data TX-5 was added to Mempool!\n");
    printf("4. Data TX-4 was added to Mempool!\n\n");

    printf("UNDO\n\n");

    printf("1. Data TX-6 was added to Mempool!\n");
    printf("2. Data TX-5 was added to Mempool!\n");
    printf("3. Data TX-4 was added to Mempool!\n\n");

    
    printf("UNDO\n\n");

    printf("1. Data TX-5 was added to Mempool!\n");
    printf("2. Data TX-4 was added to Mempool!\n\n");

    
    printf("REDO\n\n");

    printf("1. Data TX-6 was added to Mempool!\n");
    printf("2. Data TX-5 was added to Mempool!\n");
    printf("3. Data TX-4 was added to Mempool!\n\n");
    
    printf("MINING - 2 Transaction\n");
    printf("1. Data TX-6 was added to Mempool!\n\n");

    pause_screen();
}

void about_merkle_tree()
{
    clear_screen();
    printf("%s====================================================\n", GREEN);
    printf("                    ABOUT MERKLE TREE                      \n");
    printf("====================================================%s\n", WHITE);

    printf("Merkle Tree is a type of binary tree used in blockchain\n");
    printf("to efficiently and securely verify the integrity of data.\n");
    printf("Each leaf node represents the hash of a data block (e.g., transaction),\n");
    printf("and each parent node is the hash of its two children.\n");
    printf("\n");

    printf("Advantages of Merkle Tree in blockchain systems:\n");
    printf(" - Efficient verification of data integrity\n");
    printf(" - Reduces the need to download the full dataset\n");
    printf(" - Commonly used in Bitcoin, Ethereum, and many blockchains\n");
    printf("\n");

    printf("     EXAMPLE IF MERKLE TREE HAVE 4 TRANSACTION       \n");
    printf("                                                     \n");
    printf("               _________T[1234]__________            \n");
    printf("              /                         \\           \n");
    printf("         ___T[12]___              ___T[34]___        \n");
    printf("        /           \\            /          \\      \n");
    printf("     T[1]          T[2]        T[3]         T[4]     \n");
    printf("                                                     \n");

    printf("                EXAMPLE IF MERKLE TREE HAVE 5 TRANSACTION                   \n");
    printf("                                                                            \n");
    printf("                    ____________ T[12345] ____________                      \n");
    printf("                   /                                 \\                     \n");
    printf("             ____T[1234]____                     ____T[5555]______          \n");
    printf("            /               \\                  /                \\         \n");
    printf("        _T[12]_            _T[34]_          _T[55]_             _T[55]_     \n");
    printf("       /       \\         /       \\         /       \\            /         \\   \n");
    printf("     T[1]       T[2]    T[3]      T[4]   T[5]      T[5]      T[5]      T[5] \n");
    printf("                                                                            \n");
    printf("Merkle Tree Mathematical Structure:\n");
    printf("\n");

    // Leaf node calculation
    printf("Let n = number of transactions (leaf nodes required)\n");
    printf("Let m = smallest power of 2 such that m >= n\n");
    printf("        => m = 2^ceil(log2(n))\n");
    printf("If n is not a power of 2, duplicate the last transaction (T[n]) until total is m.\n");
    printf("\n");

    // Total node calculation
    printf("Total number of nodes in a full Merkle tree:\n");
    printf("        total_nodes = 2 * m - 1\n");
    printf("\n");

    // Merkle root construction
    printf("Each internal node is calculated as:\n");
    printf("        Hash(parent) = Hash(Hash(left_child) + Hash(right_child))\n");
    printf("\n");

    // Example with n = 5
    printf("Example:\n");
    printf("        n = 5\n");
    printf("        => m = 2^3 = 8  (since 8 >= 5 and 8 is next power of 2)\n");
    printf("        => total_nodes = 2 * 8 - 1 = 15\n");
    printf("        => add 3 duplicate transactions: T[5], T[5], T[5]\n");
    printf("\n");

    pause_screen();
}

void about_blockchain()
{
    clear_screen();

    printf("%s====================================================\n", GREEN);
    printf("                    ABOUT BLOCKCHAIN                    \n");
    printf("====================================================%s\n", WHITE);
    printf("\n");
    printf("A blockchain is a decentralized, distributed ledger that\n");
    printf("records transactions across many computers. This technology\n");
    printf("ensures that the data is secure, transparent, and immutable.\n\n");

    printf("Each block in the chain contains a list of data (it can be str or tx), a\n");
    printf("reference to the previous block, and a unique identifier.\n\n");

    printf("%sKey Features of Blockchain:%s\n", GREEN, WHITE);
    printf("- Decentralization: No single entity controls the network\n");
    printf("- Immutability: Once data is recorded, it cannot be altered\n");
    printf("- Transparency: All transactions are visible to participants\n");
    printf("- Security: Cryptographic techniques ensure data integrity\n");
    printf("- Consensus: Network agreement on the validity of transactions\n\n");

    printf("%sHow it works in our program:%s\n", GREEN, WHITE);
    printf("This simulation implements a simplified blockchain with:\n");
    printf("- A chain of blocks, each containing transaction data\n");
    printf("- Hashing to ensure data integrity\n");
    printf("- A mempool to queue pending transactions\n");
    printf("- Mining to validate and add blocks to the chain\n");
    printf("- Merkle trees for efficient transaction verification\n");
    printf("- Our (Users) can manipulate data and try some features\n\n");

    printf("Visual Representation:\n");
    printf("=========================================================\n");
    printf("| BLOCK #1 |->| BLOCK #2 |->| BLOCK #3 |-> ... ->| BLOCK #N |\n");
    printf("=========================================================\n");
    printf("Each block contains:\n");
    printf("- Block ID\n");
    printf("- Nonce\n");
    printf("- Transaction Data (via Merkle Tree)\n");
    printf("- Previous Block Hash\n");
    printf("- Current Block Hash\n\n");

    printf("%sNote:%s This is a simplified simulation. Real-world blockchains\n", RED, WHITE);
    printf("involve complex consensus mechanisms (e.g., Proof of Work,\n");
    printf("Proof of Stake), peer-to-peer networks, and advanced\n");
    printf("cryptographic protocols like SHA-256.\n");

    pause_screen();
}

void about_tour()
{
    about_this_program();
    about_hash(); 
    about_block(); 
    about_nonce();
    about_mining();
    about_block_tx();
    about_mempool(); 
    about_stack_log_transaction();
    about_merkle_tree(); 
    about_blockchain(); 
}

