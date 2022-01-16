#include "bst.h"
#include "ht.h"
#include "salts.h"
#include "speck.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

uint64_t lookups = 0;

struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    Node **trees;
};

HashTable *ht_create(uint32_t size) {
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    if (ht) {
        ht->salt[0] = SALT_HASHTABLE_LO;
        ht->salt[1] = SALT_HASHTABLE_HI;
        ht->size = size;
        ht->trees = (Node **) calloc(size, sizeof(Node *));
        if (!ht->trees) {
            free(ht);
            ht = NULL;
        }
    }
    return ht;
}

void ht_delete(HashTable **ht) {
    if (*ht && (*ht)->trees) {
        bst_delete((*ht)->trees);
        free(*ht);
        *ht = NULL;
    }
    return;
}

uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

Node *ht_lookup(HashTable *ht, char *oldspeak) {
    lookups += 1;

    int index = hash(ht->salt, oldspeak) % ht_size(ht);

    return bst_find(ht->trees[index], oldspeak);
}

void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {

    lookups += 1;

    int index = hash(ht->salt, oldspeak) % ht_size(ht);
    ht->trees[index] = bst_insert(ht->trees[index], oldspeak, newspeak);
    return;
}

uint32_t ht_count(HashTable *ht) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < ht_size(ht); i += 1) {
        if (ht->trees[i] != NULL) {
            count += 1;
        }
    }
    return count;
}

double ht_avg_bst_size(HashTable *ht) {
    double size = 0;
    double avg_size = 0;
    for (uint32_t i = 0; i < ht_size(ht); i += 1) {
        size += (double) bst_size(ht->trees[i]);
    }
    avg_size = size / (double) ht_count(ht);
    return avg_size;
}

double ht_avg_bst_height(HashTable *ht) {
    double height = 0;
    double avg_height = 0;
    for (uint32_t i = 0; i < ht_size(ht); i += 1) {
        height += (double) bst_height(ht->trees[i]);
    }
    avg_height = height / (double) ht_count(ht);
    return avg_height;
}

void ht_print(HashTable *ht) {
    for (uint32_t i = 0; i < ht_size(ht); i += 1) {
        bst_print(ht->trees[i]);
        printf("\n");
    }

    double load = 100 * ((double) ht_count(ht) / (double) ht_size(ht));

    printf("ht load: %.6f\n", load);

    printf("ht size: %" PRIu32 "\n", ht_size(ht));
    printf("ht count %" PRIu32 "\n", ht_count(ht));
    printf("ht avg bst size: %f \n", ht_avg_bst_size(ht));
    printf("ht avg bst height: %f \n", ht_avg_bst_height(ht));
}
