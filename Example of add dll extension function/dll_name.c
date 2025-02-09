#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// void function(void);

#define add_instructions //function, ...

void (*new_funcs[])() = {add_instructions};
int new_funcs_size = sizeof(new_funcs) / sizeof(new_funcs[0]);

#define INCLUDE_LOADFUNCS

#include "loadfuncs.c"

// void function(void){
//     // Do Something
// }
