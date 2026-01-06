#ifndef RECORD_H
#define RECORD_H

typedef struct {
    // number of columns in this record (same as headers count)
    int nfields;
    // malloc'd array of malloc'd strings (exact length)
    char **fields;
} Record;

#endif
