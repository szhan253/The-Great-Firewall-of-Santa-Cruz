#include "bv.h"
#include "bf.h"
#include "salts.h"
#include "speck.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

struct BloomFilter {
    uint64_t primary[2]; // Primary hash function salt .
    uint64_t secondary[2]; // Secondary hash function salt .
    uint64_t tertiary[2]; // Tertiary hash function salt .
    BitVector *filter;
};

BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    if (bf) {
        bf->primary[0] = SALT_PRIMARY_LO;
        bf->primary[1] = SALT_PRIMARY_HI;
        bf->secondary[0] = SALT_SECONDARY_LO;
        bf->secondary[1] = SALT_SECONDARY_HI;
        bf->tertiary[0] = SALT_TERTIARY_LO;
        bf->tertiary[1] = SALT_HASHTABLE_HI;

        bf->filter = bv_create(size);
        if (!bf->filter) {
            free(bf);
            bf = NULL;
        }
    }

    return bf;
}

void bf_delete(BloomFilter **bf) {
    if (*bf && (*bf)->filter) {
        bv_delete(&(*bf)->filter);
        free(*bf);
        *bf = NULL;
    }
    return;
}

uint32_t bf_size(BloomFilter *bf) {
    uint32_t size = bv_length(bf->filter);
    return size;
}

void bf_insert(BloomFilter *bf, char *oldspeak) {
    //get three indexs
    uint32_t pr_index = hash(bf->primary, oldspeak) % bf_size(bf);
    uint32_t sec_index = hash(bf->secondary, oldspeak) % bf_size(bf);
    uint32_t ter_index = hash(bf->tertiary, oldspeak) % bf_size(bf);

    //insert into three different indexs
    bv_set_bit(bf->filter, pr_index);
    bv_set_bit(bf->filter, sec_index);
    bv_set_bit(bf->filter, ter_index);

    return;
}

bool bf_probe(BloomFilter *bf, char *oldspeak) {
    //hash three times
    uint32_t pr_index = hash(bf->primary, oldspeak) % bf_size(bf);
    uint32_t sec_index = hash(bf->secondary, oldspeak) % bf_size(bf);
    uint32_t ter_index = hash(bf->tertiary, oldspeak) % bf_size(bf);

    //get bit three times
    bool pr = bv_get_bit(bf->filter, pr_index);
    bool sec = bv_get_bit(bf->filter, sec_index);
    bool ter = bv_get_bit(bf->filter, ter_index);

    //check if get bit conditions
    if (pr == true && sec == true && ter == true) {
        return true;
    }

    return false;
}

uint32_t bf_count(BloomFilter *bf) {
    uint32_t count = 0;

    for (uint32_t i = 0; i < bf_size(bf); i += 1) {
        if (bv_get_bit(bf->filter, i) == true) {
            count += 1;
        }
    }

    return count;
}

void bf_print(BloomFilter *bf) {
    bv_print(bf->filter);
    return;
}
