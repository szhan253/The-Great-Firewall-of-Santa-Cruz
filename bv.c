#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "bv.h"

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

//constructor
BitVector *bv_create(uint32_t length) {
    BitVector *bv = (BitVector *) malloc(sizeof(BitVector));
    //BitVector bv;
    if (bv) {
        bv->length = length;
        bv->vector = (uint8_t *) calloc(length, sizeof(uint8_t));
        if (!bv->vector) {
            free(bv);
            bv = NULL;
        }
    }
    return bv;
}

//destructor
void bv_delete(BitVector **bv) {
    if ((*bv) && (*bv)->vector) {
        free((*bv)->vector);
        free(*bv);
        *bv = NULL;
    }

    return;
}

//return bv's length
uint32_t bv_length(BitVector *bv) {
    return bv->length;
}

bool bv_set_bit(BitVector *bv, uint32_t i) {
    //check if i is in the range
    if (i < bv_length(bv)) {
        //if i is in the range, set bit and return true
        bv->vector[i / 8] |= (0x1 << i % 8);
        return true;
    } else {
        //if i is out of range, return false
        return false;
    }
}

bool bv_clr_bit(BitVector *bv, uint32_t i) {
    //check if i is in the range
    if (i < bv_length(bv)) {
        //if i is in the range, clear bit and return true
        bv->vector[i / 8] &= ~(0x1 << (i % 8));
        return true;
    } else {
        //if i is out of range, return false
        return false;
    }
}

bool bv_get_bit(BitVector *bv, uint32_t i) {
    //if i is in the range
    if (i < bv_length(bv)) {
        //if the value is 1, return true
        if (((bv->vector[i / 8] >> (i % 8)) & 1) == 1) {
            return true;
        }
        //if the value is 0
        return false;
    }
    //if i is out of range
    return false;
}

void bv_print(BitVector *bv) {
    printf("Bit length: %" PRIu32 "\n", bv_length(bv));
    for (uint32_t i = 0; i < bv_length(bv); i++) {
        if (bv_get_bit(bv, i) == true) {
            printf("1 ");
        } else {
            printf("0 ");
        }
        //printf("Bits: %" PRIu8 "\n", )
    }
    printf("\n");
}
