#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>

// void Example_function(void);

#define add_instructions //Example_function

void (*new_funcs[])() = {add_instructions};
int new_funcs_size = sizeof(new_funcs) / sizeof(new_funcs[0]);

#define INCLUDE_LOADFUNCS

#include "loadfuncs.c"

// void Example_function(void){
//     printf("Executed:Example_function\n");
//     *crt_ptr += 0 + sizeof(int);
// }