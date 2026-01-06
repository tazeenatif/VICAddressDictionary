#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"
#include "bitstring.h"

Dictionary *dict_create(char **headers, int nheaders, int key_index) {
    Dictionary *d = malloc(sizeof(Dictionary));
    if (d == NULL) {
        // malloc failed, caller must handle
        return NULL;
    }
    d->head = d->tail = NULL;
    d->size = 0;
    d->nheaders = nheaders;
    // free in dict_free
    d->headers = headers;
    d->key_index = key_index;
    return d;
}

void dict_insert(Dictionary *dict, Record *rec) {
    Node *node = malloc(sizeof(Node));
    if (node == NULL) {
        // failed allocation, so just return, dict unchanged
        return;
    }
    node->rec = rec;
    node->next = NULL;
    if (!dict->head) {
        dict->head = node;
    }
    else {
        dict->tail->next = node;
    }
    dict->tail = node;
    dict->size++;
}

void dict_free(Dictionary *dict) {
    // free all records
    Node *cur = dict->head;
    while (cur) {
        Record *r = cur->rec;
        for (int i = 0; i < r->nfields; i++) {
            free(r->fields[i]);
        }
        free(r->fields);
        free(r);
        Node *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
    // free headers
    for (int i = 0; i < dict->nheaders; i++) {
        free(dict->headers[i]);
    }
    free(dict->headers);
    free(dict);
}

// write a record in the expected labeled format:
/* --> HEADER: VALUE || HEADER: VALUE || ... y: ... || 
*/
static void write_record(FILE *out, const Dictionary *dict, const Record *rec) {
    fprintf(out, "--> ");
    for (int i = 0; i < dict->nheaders; i++) {
        const char *label = dict->headers[i];
        const char *val = (i < rec->nfields && rec->fields[i]) ? rec->fields[i] : "";
        fprintf(out, "%s: %s", label, val);
        fprintf(out, " || ");
    }
    fprintf(out, "\n");
}

int dict_search_write(Dictionary *dict,
                      const char *key,
                      FILE *out,
                      long long *bit_comparisons,
                      int *node_comparisons,
                      int *string_comparisons) {
    long long bsum = 0;
    int nsum = 0;
    int ssum = 0;
    int matches = 0;

    for (Node *cur = dict->head; cur; cur = cur->next) {
        nsum++;
        long long b = 0;
        int cmp = bitStringCompare(cur->rec->fields[dict->key_index], key, &b);
        bsum += b;
        // one string compare per node in linear search
        ssum++;
        if (cmp == 0) {
            write_record(out, dict, cur->rec);
            matches++;
        }
    }

    if (bit_comparisons) {
        *bit_comparisons = bsum;
    }
    if (node_comparisons) {
        *node_comparisons = nsum;
    }
    if (string_comparisons) {
        *string_comparisons = ssum;
    }
    return matches;
}
