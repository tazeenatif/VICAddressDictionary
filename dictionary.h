#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdio.h>
#include "record.h"

// linked-list node
typedef struct Node {
    Record *rec;
    struct Node *next;
} Node;

// dictionary holding records and column metadata
typedef struct {
    Node *head;
    Node *tail;
    int size;            // number of records
    int nheaders;        // number of columns
    char **headers;      // header names (malloc'd strings)
    int key_index;       // index of "EZI_ADD" column
} Dictionary;

Dictionary *dict_create(char **headers, int nheaders, int key_index);
void dict_insert(Dictionary *dict, Record *rec);
void dict_free(Dictionary *dict);

// linear search by key; writes matches to 'out' using labeled fields.
// also accumulates counts for bit comparisons, node comparisons, string comparisons.
// returns number of matches.
int dict_search_write(Dictionary *dict,
                      const char *key,
                      FILE *out,
                      long long *bit_comparisons,
                      int *node_comparisons,
                      int *string_comparisons);

#endif
