/* main.c
 *
 * By Tazeen Atif, August 2025
 *
 * This program reads a CSV file contaning city data and performs
 * dictionary operations (insert, search, etc) based on the input.
 *
 * To run:
 *   ./program input.csv
 *
 * Input:
 *   - CSV file with lines of format: City,Population,Percentage
 *
 * Output:
 *   - Processed data, written to stdout
 *   - Returns 1 on error (like file not found, invalid data)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"
#include "csv.h"

#define MAX_LINE 1024

// trim trailing newline and carriage return
static void strip_newline(char *s) {
    size_t n = strlen(s);
    while (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r')) {
        s[--n] = '\0';
    }
}

// duplicate string with exact length
static char *xstrdup(const char *s) {
    size_t n = strlen(s);
    char *p = malloc(n + 1);
    if (!p) {
        exit(1);
    }
    memcpy(p, s, n + 1);
    return p;
}

static int find_header_index(char **headers, int n, const char *name) {
    for (int i = 0; i < n; i++) {
        if (strcmp(headers[i], name) == 0) return i;
    }
    return -1;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s mode input.csv output.txt\n", argv[0]);
        return 1;
    }

    const char *input_csv = argv[2];
    const char *output_txt = argv[3];

    FILE *in = fopen(input_csv, "r");
    if (!in) {
        return 1;
    }
    FILE *out = fopen(output_txt, "w");
    if (!out) {
        fclose(in);
        return 1;
    }

    // read header line
    char line[MAX_LINE];
    if (!fgets(line, sizeof(line), in)) {
        fclose(in); 
        fclose(out);
    }
    strip_newline(line);

    // parse header
    int nheaders_raw = 0;
    char **raw_fields = csv_split_line(line, &nheaders_raw);

    // normalise header fields (strip quotes, etc.)
    char **headers = malloc(sizeof(char*) * nheaders_raw);
    if (!headers) { 
        exit(1); 
    }

    for (int i = 0; i < nheaders_raw; i++) {
        headers[i] = csv_normalise_field(raw_fields[i]);
    }
    csv_free_fields(raw_fields, nheaders_raw);

    // find EZI_ADD index
    int key_index = find_header_index(headers, nheaders_raw, "EZI_ADD");
    if (key_index < 0) {
        fclose(in); fclose(out);
    }

    // create dictionary
    Dictionary *dict = dict_create(headers, nheaders_raw, key_index);

    // read each data row once, then build linked list in input order
    while (fgets(line, sizeof(line), in)) {
        strip_newline(line);
        // skip empty lines
        if (line[0] == '\0') {
            continue;
        }

        int nf = 0;
        char **raw = csv_split_line(line, &nf);

        // normalise all fields
        char **norm = malloc(sizeof(char*) * nf);
        if (!norm) { 
            exit(1);
        }

        for (int i = 0; i < nf; i++) {
            norm[i] = csv_normalise_field(raw[i]);
        }
        csv_free_fields(raw, nf);

        // build record
        Record *rec = malloc(sizeof(Record));
        if (!rec) { 
            exit(1);
        }

        rec->nfields = nf;
        rec->fields = norm;

        // if row has fewer fields than headers, pad with empty strings
        if (nf < dict->nheaders) {
            char **newarr = malloc(sizeof(char*) * dict->nheaders);
            if (!newarr) { 
                exit(1);
            }
            for (int i = 0; i < nf; i++) {
                newarr[i] = rec->fields[i];
            }
            for (int i = nf; i < dict->nheaders; i++) {
                newarr[i] = xstrdup("");
            }
            free(rec->fields);
            rec->fields = newarr;
            rec->nfields = dict->nheaders;
        } 
        else if (nf > dict->nheaders) {
            // truncate to header count for consistent output
            for (int i = dict->nheaders; i < nf; i++) {
                free(rec->fields[i]);
            }
            rec->nfields = dict->nheaders;
        }

        dict_insert(dict, rec);
    }
    fclose(in);

    // process queries from stdin until EOF
    char query[MAX_LINE];
    while (fgets(query, sizeof(query), stdin)) {
        strip_newline(query);
        if (query[0] == '\0') {
            continue;
        }

        // write the query to output file first
        fprintf(out, "%s\n", query);

        long long bcmp = 0;
        int ncmp = 0, scmp = 0;
        int matches = dict_search_write(dict, query, out, &bcmp, &ncmp, &scmp);

        // write the summary to stdout
        printf("%s --> %d records found - comparisons: b%lld n%d s%d\n",
               query, matches, bcmp, ncmp, scmp);
        fflush(stdout);
    }

    dict_free(dict);
    fclose(out);
    return 0;
}
