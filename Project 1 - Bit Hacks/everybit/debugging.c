// Here we make a simple main program to debug the algorithms

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./bitarray.h"
#include "./tests.h"

int main(int argc, char** argv) {
    char bitstring_value = '10010110';
    const char* bitstring = &bitstring_value;
    const size_t bitstring_length = strlen(bitstring);

    static bitarray_t* test_bitarray = NULL;
    test_bitarray = bitarray_new(bitstring_length);

    bool current_bit;
    for (size_t i = 0; i < bitstring_length; i++) {
        current_bit = (bitstring[i] == '1');
        bitarray_set(test_bitarray, i, current_bit);
    }
    bitarray_fprint(stdout, test_bitarray);

    //bitarray_rotate(test_bitarray, bit_offset, bit_length, bit_right_shift_amount);
    return 0;
}