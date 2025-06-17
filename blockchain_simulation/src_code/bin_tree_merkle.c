#include "bin_tree_merkle.h"
#include "hash_engine.h"

merkle_node* create_merkle_node(char* hash, char* label, bool is_tainted) {
    merkle_node* node = (merkle_node*)malloc(sizeof(merkle_node));
    strcpy(node->hash, hash);
    strcpy(node->label, label);
    node->is_tainted = is_tainted;
    node->left = node->right = NULL;
    return node;
}

void generate_tx_hash(tx* transaction) {
    char data[MAX_STRING * 2];
    sprintf(data, "%d%s%d%s%u", transaction->sender_id, transaction->sender_name, transaction->receiver_id, transaction->receiver_name, transaction->transfer_amount);
    hash_generator_tx(data, transaction->tx_hash);
}

merkle_node* build_merkle_tree(tx transactions[], int count) {
    if (count == 0) return NULL;
    for(int i=0; i<count; i++) { generate_tx_hash(&transactions[i]); }

    int node_count = count;
    merkle_node** leaves = (merkle_node**)malloc(node_count * sizeof(merkle_node*));
    for (int i = 0; i < count; i++) {
        char label[MAX_STRING];
        sprintf(label, "Tx[%d]", transactions[i].num);
        leaves[i] = create_merkle_node(transactions[i].tx_hash, label, transactions[i].is_edited);
    }

    if (node_count % 2 != 0 && node_count > 0) {
        leaves = (merkle_node**)realloc(leaves, (node_count + 1) * sizeof(merkle_node*));
        char new_label[MAX_STRING];
        sprintf(new_label, "%s'", leaves[node_count-1]->label);
        leaves[node_count] = create_merkle_node(leaves[node_count - 1]->hash, new_label, leaves[node_count - 1]->is_tainted);
        node_count++;
    }

    while (node_count > 1) {
        merkle_node** parents = (merkle_node**)malloc((node_count / 2) * sizeof(merkle_node*));
        int parent_idx = 0;
        for (int i = 0; i < node_count; i += 2) {
            char combined_hash_data[HASH_SIZE * 2 + 1];
            strcpy(combined_hash_data, leaves[i]->hash);
            strcat(combined_hash_data, leaves[i+1]->hash);
            char parent_hash[HASH_SIZE];
            hash_generator_tx(combined_hash_data, parent_hash);
            bool parent_is_tainted = leaves[i]->is_tainted || leaves[i+1]->is_tainted;
            
            char parent_label[50];
            char L_label_content[20] = "", R_label_content[20] = "";
            sscanf(leaves[i]->label, "H(%[^)])", L_label_content);
            if(strlen(L_label_content) == 0) sscanf(leaves[i]->label, "Tx[%[^]]]", L_label_content);
            sscanf(leaves[i+1]->label, "H(%[^)])", R_label_content);
            if(strlen(R_label_content) == 0) sscanf(leaves[i+1]->label, "Tx[%[^]]]", R_label_content);
            
            char* L_clean = strchr(L_label_content, '\''); if (L_clean) *L_clean = '\0';
            char* R_clean = strchr(R_label_content, '\''); if (R_clean) *R_clean = '\0';
            
            sprintf(parent_label, "H(%s%s)", L_label_content, R_label_content);

            merkle_node* parent = create_merkle_node(parent_hash, parent_label, parent_is_tainted);
            parent->left = leaves[i]; parent->right = leaves[i+1];
            parents[parent_idx++] = parent;
        }
        if (node_count > 1) free(leaves);
        leaves = parents;
        node_count /= 2;
        if (node_count % 2 != 0 && node_count > 1) {
             leaves = (merkle_node**)realloc(leaves, (node_count + 1) * sizeof(merkle_node*));
             char new_label[MAX_STRING];
             sprintf(new_label, "%s'", leaves[node_count-1]->label);
             leaves[node_count] = create_merkle_node(leaves[node_count - 1]->hash, new_label, leaves[node_count - 1]->is_tainted);
             node_count++;
        }
    }
    merkle_node* root = leaves[0];
    strcpy(root->label, "Merkle Root");
    free(leaves);
    return root;
}

void print_merkle_tree_visual(merkle_node* node, int level, char* prefix) {
    if (node == NULL) return;
    printf("%s%s", prefix, (level > 0) ? "|-- " : "");
    const char* color = node->is_tainted ? RED : GREEN;
    printf("%s%s (%s)%s\n", color, node->label, node->hash, WHITE);
    
    char child_prefix[MAX_STRING];
    strcpy(child_prefix, prefix);
    strcat(child_prefix, (node->right != NULL) ? "|   " : "    ");
    
    print_merkle_tree_visual(node->left, level + 1, child_prefix);
    
    strcpy(child_prefix, prefix);
    strcat(child_prefix, "    ");
    print_merkle_tree_visual(node->right, level + 1, child_prefix);
}

void free_merkle_tree(merkle_node* node) {
    if (node == NULL) return;
    free_merkle_tree(node->left);
    free_merkle_tree(node->right);
    free(node);
}