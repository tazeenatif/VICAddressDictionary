#include <string.h>
#include "bitstring.h"

int getBit(const char *s, int pos) {
    int byte = pos / 8;
    // 0 = MSB of THIS byte
    int bit  = pos % 8;
    unsigned char c = (unsigned char)s[byte];
    return (c >> (7 - bit)) & 1;
}

int bitStringCompare(const char *a, const char *b, long long *count) {
    int lenA = (int)strlen(a);
    int lenB = (int)strlen(b);
    // include null terminator
    int bitsA = (lenA + 1) * 8;
    int bitsB = (lenB + 1) * 8;
    int maxBits = bitsA > bitsB ? bitsA : bitsB;

    for (int i = 0; i < maxBits; i++) {
        int bitA = (i < bitsA) ? getBit(a, i) : 0;
        int bitB = (i < bitsB) ? getBit(b, i) : 0;
        (*count)++;
        if (bitA < bitB) {
            return -1;
        }
        if (bitA > bitB) {
            return 1;
        }
    }
    return 0;
}
