#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "node.h"

Node *node_create(char *oldspeak, char *newspeak) {
    Node *n = (Node *) malloc(sizeof(Node));
    if (n) {
        if (oldspeak == NULL) {
            n->oldspeak = NULL;
        } else {
            char *tempold = strdup(oldspeak);
            n->oldspeak = tempold;
        }

        if (newspeak == NULL) {
            n->newspeak = NULL;
        } else {
            char *tempnew = strdup(newspeak);
            n->newspeak = tempnew;
        }

        n->left = NULL;
        n->right = NULL;
    }
    return n;
}
void node_delete(Node **n) {
    if (*n) {

        free((*n)->oldspeak);
        free((*n)->newspeak);
        free((*n)->left);
        free((*n)->right);
        free(*n);
        *n = NULL;
    }

    return;
}

void node_print(Node *n) {
    if (n->oldspeak != NULL && n->newspeak != NULL) {
        printf("%s -> %s\n", n->oldspeak, n->newspeak);
    } else if (n->oldspeak != NULL && n->newspeak == NULL) {
        printf("%s\n", n->oldspeak);
    }
}
