#ifndef BITSTRING_H
#define BITSTRING_H

// return the bit at absolute bit position 'pos' in string s (including '\0')
int getBit(const char *s, int pos);

// bitwise string comparison (includes '\0' as part of comparison).
// returns -1, 0, 1 (like strcmp). writes total bit comparisons into *count.
int bitStringCompare(const char *a, const char *b, long long *count);

#endif
