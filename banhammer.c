#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <math.h>
#include <regex.h>
#include <ctype.h>

#include "messages.h"
#include "salts.h"
#include "speck.h"
#include "ht.h"
#include "bst.h"
#include "node.h"
#include "bf.h"
#include "bv.h"
#include "parser.h"

#define OPTIONS "ht:f:s"

#define WORD "[a-zA-Z0-9_'-]+"

int main(int argc, char **argv) {

    int opt = 0;
    //default sizes
    uint32_t htSize = pow(2, 16);
    uint32_t bfSize = pow(2, 20);

    bool test_h = false;
    bool test_t = false;
    bool test_f = false;
    bool test_s = false;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': test_h = true; break;

        case 't':
            test_t = true;
            htSize = atoi(optarg);
            break;

        case 'f':
            test_f = true;
            bfSize = atoi(optarg);
            break;

        case 's': test_s = true; break;

        default: test_h = true;
        }
    }

    //helping message
    if (test_h) {
        printf("SYNOPSIS\n");
        printf("  A word filtering program for the GPRSC.\n");
        printf("  Filters out and reports bad words parsed from stdin.\n\n");

        printf("USAGE\n");
        printf("  ./banhammer [-hs] [-t size] [-f size]\n\n");

        printf("OPTIONS\n");
        printf("  -h           Program usage and help.\n");
        printf("  -s           Print program statistics.\n");
        printf("  -t size      Specify hash table size (default: 2^16).\n");
        printf("  -f size      Specify Bloom filter size (default: 2^20).\n");
        exit(0);
    }

    BloomFilter *bf = bf_create(bfSize);
    HashTable *ht = ht_create(htSize);
    Node *n;
    Node *bst = bst_create();
    Node *new_bst = bst_create();

    //input file names
    char *badfile = "badspeak.txt";
    char *newfile = "newspeak.txt";

    bool old_check = false;
    bool new_check = false;

    //open files
    FILE *bfil = fopen(badfile, "r");
    FILE *nfil = fopen(newfile, "r");

    //buffer for new and old speak
    char old_buf[1024]; // = {'\0'};
    char new_buf[1024]; // = {'\0'};
    char bad_buf[1024];

    //read from badspeak
    while (fscanf(bfil, "%s", bad_buf) != EOF) {
        bf_insert(bf, bad_buf);
        ht_insert(ht, bad_buf, NULL);
    }

    //read from newspeak
    while (fscanf(nfil, "%s %s", old_buf, new_buf) != EOF) {
        bf_insert(bf, old_buf);
        ht_insert(ht, old_buf, new_buf);
    }

    //from asgn pdf
    regex_t re;
    if (regcomp(&re, WORD, REG_EXTENDED)) {
        fprintf(stderr, "Failed to compile regex .\n");
        return 1;
    }

    char *word = NULL;
    while ((word = next_word(stdin, &re)) != NULL) {
        for (uint32_t i = 0; word[i]; i += 1) {
            word[i] = tolower(word[i]);
        }
        //check if bf has word
        if (bf_probe(bf, word)) {
            //check if ht has word
            n = ht_lookup(ht, word);
            //if has
            if (n) {
                if (n->newspeak == NULL) {
                    bst = bst_insert(bst, n->oldspeak, NULL); //thoughtcrime: only old speak
                    old_check = true;
                } else {
                    new_bst = bst_insert(
                        new_bst, n->oldspeak, n->newspeak); //Rightspeak: old && new speak
                    new_check = true;
                }
            }
        }
    }

    //print statistics information
    if (test_s) {
        printf("Average BST size: %.6f\n", ht_avg_bst_size(ht));
        printf("Average BST height: %.6f\n", ht_avg_bst_height(ht));

        double avg_branches = (double) branches / (double) lookups;
        printf("Average branches traversed: %.6f\n", avg_branches);

        double ht_load = 100 * ((double) ht_count(ht) / (double) ht_size(ht));
        printf("Hash table load: %.6f%%\n", ht_load);

        double bf_load = 100 * ((double) bf_count(bf) / (double) bf_size(bf));
        printf("Bloom filter load: %.6f%%\n", bf_load);
        exit(0);
    }

    //check speak stats, output message
    if (old_check && new_check) {
        //mixspeak
        printf("%s", mixspeak_message);
        bst_print(bst);
        bst_print(new_bst);
    } else if (old_check && !new_check) {
        //badspeak message
        printf("%s", badspeak_message);
        bst_print(bst);
    } else if (!old_check && new_check) {
        //goodspeak message
        printf("%s", goodspeak_message);
        bst_print(new_bst);
    } else {
        printf("good\n");
    }

    //free memory
    clear_words();
    regfree(&re);
    bf_delete(&bf);
    ht_delete(&ht);
    node_delete(&n);
    bst_delete(&bst);
    bst_delete(&new_bst);

    //close files
    fclose(bfil);
    fclose(nfil);

    return 0;
}
