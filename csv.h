#ifndef CSV_H
#define CSV_H

#include "record.h"

// split one CSV line into an array of malloc'd strings (fields) and write count.
// handles quoted fields and escaped double quotes (""). No multiline support.
char **csv_split_line(const char *line, int *out_count);

// strip surrounding quotes and unescape double quotes if present.
// returns a newly malloc'd string.
char *csv_normalise_field(const char *src);

// free a list of strings created by csv_split_line.
void csv_free_fields(char **fields, int count);

#endif
