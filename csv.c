#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "csv.h"

char *csv_normalise_field(const char *src) {
    size_t len = strlen(src);
    // trim trailing CR/LF (safety guard)
    while (len > 0 && (src[len - 1] == '\r' || src[len - 1] == '\n')) len--;

    if (len >= 2 && src[0] == '"' && src[len - 1] == '"') {
        // strip outer quotes and unescape "" -> "
        const char *p = src + 1;
        const char *end = src + len - 1;
        // worst-case length
        char *out = malloc((len - 1) + 1);
        if (!out) {
            exit(1);
        }

        size_t j = 0;
        while (p < end) {
            if (*p == '"' && (p + 1) < end && *(p + 1) == '"') {
                out[j++] = '"';
                p += 2;
            } 
            else {
                out[j++] = *p++;
            }
        }
        out[j] = '\0';
        return out;
    } 
    else {
        // no surrounding quotes, copy as-is
        char *out = malloc(len + 1);
        if (!out) {
            exit(1);
        }

        memcpy(out, src, len);
        out[len] = '\0';
        return out;
    }
}
/* idea and implementation from Stack Overflow
https://stackoverflow.com/questions/12911299/read-csv-file-in-c */
char **csv_split_line(const char *line, int *out_count) {
    int cap = 16;
    int count = 0;
    char **fields = malloc(sizeof(char*) * cap);
    if (!fields) exit(1);

    const char *p = line;
    while (*p) {
        // resize if needed
        if (count >= cap) {
            cap *= 2;
            char **new_fields = realloc(fields, sizeof(char*) * cap);
            if (!new_fields) {
                exit(1);
            }
            fields = new_fields;
        }

        // parse one field
        if (*p == '"') {
            //skip opening quote
            p++;
            size_t bufcap = 64, buflen = 0;
            char *buf = malloc(bufcap);
            if (!buf) {
                exit(1);
            }

            while (*p) {
                if (*p == '"') {
                    if (*(p + 1) == '"') {
                        // escaped quote
                        if (buflen + 1 >= bufcap) {
                            bufcap *= 2;
                            char *new_buf = realloc(buf, bufcap);
                            if (!new_buf) {
                                exit(1);
                            }
                            buf = new_buf;
                        }
                        buf[buflen++] = '"';
                        p += 2;
                    } 
                    else {
                        //skip closing quote
                        p++;
                        break;
                    }
                } 
                else {
                    if (buflen + 1 >= bufcap) {
                        bufcap *= 2;
                        char *new_buf = realloc(buf, bufcap);
                        if (!new_buf) {
                            exit(1);
                        }
                        buf = new_buf;
                    }
                    buf[buflen++] = *p++;
                }
            }
            buf[buflen] = '\0';
            if (*p == ',') {
                p++;
            }
            fields[count++] = buf;
        }
        else {
            // unquoted field
            const char *start = p;
            while (*p && *p != ',') {
                p++;
            }
            size_t len = (size_t)(p - start);

            char *buf = malloc(len + 1);
            if (!buf) {
                exit(1);
            }

            memcpy(buf, start, len);
            buf[len] = '\0';
            if (*p == ',') {
                p++;
            }
            fields[count++] = buf;
        }
    }

    *out_count = count;
    return fields;
}

void csv_free_fields(char **fields, int count) {
    for (int i = 0; i < count; i++) {
        free(fields[i]);
    }
    free(fields);
}