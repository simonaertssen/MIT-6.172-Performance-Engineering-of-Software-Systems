// Copyright (c) 2012 MIT License by 6.172 Staff

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define PRINT_SIZE(type, size) printf("size of %s : %zu bytes \n", type, size);

int main() {
  // Please print the sizes of the following types:
  // int, short, long, char, float, double, unsigned int, long long
  // uint8_t, uint16_t, uint32_t, and uint64_t, uint_fast8_t,
  // uint_fast16_t, uintmax_t, intmax_t, __int128, and student

  // Here's how to show the size of one type. See if you can define a macro
  // to avoid copy pasting this code.
  // printf("size of %s : %zu bytes \n", "int", sizeof(int));

  PRINT_SIZE("int", sizeof(int));
  PRINT_SIZE("short", sizeof(short));
  PRINT_SIZE("long", sizeof(long));
  PRINT_SIZE("char", sizeof(char));
  PRINT_SIZE("float", sizeof(float));
  PRINT_SIZE("double", sizeof(double));

  PRINT_SIZE("unsigned int", sizeof(unsigned int));
  PRINT_SIZE("long long", sizeof(long long));

  PRINT_SIZE("int8_t", sizeof(int8_t));
  PRINT_SIZE("int16_t", sizeof(int16_t));
  PRINT_SIZE("int32_t", sizeof(int32_t));
  PRINT_SIZE("int64_t", sizeof(int64_t));

  PRINT_SIZE("int_fast8_t", sizeof(int_fast8_t));
  PRINT_SIZE("int_fast16_t", sizeof(int_fast16_t));
  PRINT_SIZE("int_fast32_t", sizeof(int_fast32_t));
  PRINT_SIZE("int_fast64_t", sizeof(int_fast64_t));

  PRINT_SIZE("uint8_t", sizeof(uint8_t));
  PRINT_SIZE("uint16_t", sizeof(uint16_t));
  PRINT_SIZE("uint32_t", sizeof(uint32_t));
  PRINT_SIZE("uint64_t", sizeof(uint64_t));

  PRINT_SIZE("uint_fast8_t", sizeof(uint_fast8_t));
  PRINT_SIZE("uint_fast16_t", sizeof(uint_fast16_t));
  PRINT_SIZE("uint_fast32_t", sizeof(uint_fast32_t));
  PRINT_SIZE("uint_fast64_t", sizeof(uint_fast64_t));

  // Alternatively, you can use stringification
  // (https://gcc.gnu.org/onlinedocs/cpp/Stringification.html) so that
  // you can write
  // e.g. PRINT_SIZE(int);
  //      PRINT_SIZE(short);

  // Composite types have sizes too.
  typedef struct {
    int id;
    int year;
  } student;

  student you;
  you.id = 12345;
  you.year = 4;
  PRINT_SIZE("student", sizeof(you));

  // Array declaration. Use your macro to print the size of this.
  int x[5];
  PRINT_SIZE("int array 4", sizeof(x));

  return 0;
}
