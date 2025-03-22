#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define ERROR_LIBRARY_LOAD -1
#define ERROR_FUNCTION_LOAD -2
#define ERROR_FILE_OPEN -3
#define ERROR_FILE_READ -4
#define ERROR_NOT_ENOUGH_ARGUMENTS -5
#define ERROR_MEMORY_ALLOCATION -6
#define ERROR_MEMORY_REALLOCATION -7

void *bse, *crt;
int imp;
FILE *file;

// void function(void){
//     //function code
// }

//#define name_instruction function, ...//#define name_instruction function, ...

void OVER(void){
    exit(*((int*)crt + 1));
}
void MOV(void){
    size_t src_offset_bits = *((int*)crt + 1);
    size_t dest_offset_bits = *((int*)crt + 2);
    size_t num_bits =  *((int*)crt + 3);
    for (size_t i = 0; i < num_bits; ++i) {
        size_t src_total = src_offset_bits + i;
        size_t dest_total = dest_offset_bits + i;
        size_t src_byte = src_total / 8;
        uint8_t src_bit = 7 - (src_total % 8);
        uint8_t src_val = (((char*)bse)[src_byte] >> src_bit) & 1;
        size_t dest_byte = dest_total / 8;
        uint8_t dest_bit = 7 - (dest_total % 8);
        if (src_val) {
            ((char*)bse)[dest_byte] |= (1 << dest_bit);
        } else {
            ((char*)bse)[dest_byte] &= ~(1 << dest_bit);
        }
    }
    imp = *(int*)(crt += 3*sizeof(int)+sizeof(int));
}
void JMP(void){
    if(*(char*)(crt + 1)){
        imp = *(int*)(crt = bse + *((int*)crt + 2));
    }else{
        imp = *(int*)(crt += sizeof(char)+sizeof(int)+sizeof(int));
    }
}

void DLL(void);

#define basic_instruction OVER, MOV, JMP, DLL

void (*basic_funcs[])() = {basic_instruction};//void (*basic_funcs[])() = {basic_instruction, name_instruction, ...};
void (**funcs)() = basic_funcs;
int funcs_size = sizeof(basic_funcs) / sizeof(basic_funcs[0]);

void DLL() {
    char *err = (crt += sizeof(int));
    HINSTANCE hLib = LoadLibrary(crt);if (!hLib) { *err = ERROR_LIBRARY_LOAD; return; }
    if (funcs == basic_funcs) {
        funcs = malloc((funcs_size - 1) * sizeof(void(*)()));if (!funcs) { *err = ERROR_MEMORY_ALLOCATION; return; }
        memcpy(funcs, basic_funcs, (funcs_size - 1) * sizeof(void(*)()));
    }
    crt += strlen(crt) + 1;
    while (*(char*)crt) {
        funcs_size++;
        funcs = realloc(funcs, funcs_size * sizeof(void(*)()));if (!funcs) { *err = ERROR_MEMORY_REALLOCATION; return; }
        FARPROC proc = GetProcAddress(hLib, crt);if (!proc) { *err = ERROR_FUNCTION_LOAD; return; }
        funcs[funcs_size - 1] = (void(*)())proc;
        crt += strlen(crt) + 1;
    }
    imp = *(int*)(++crt);
}

__declspec(dllexport) void** vmc_get_bse_ptr() {return &bse;}
__declspec(dllexport) void** vmc_get_crt_ptr() {return &crt;}
__declspec(dllexport) int* vmc_get_imp_ptr() {return &imp;}
__declspec(dllexport) FILE** vmc_get_file_ptr() {return &file;}
__declspec(dllexport) void*** vmc_get_funcs_ptr() {return (void***)&funcs;}
__declspec(dllexport) int* vmc_get_funcs_size_ptr() {return &funcs_size;}
__declspec(dllexport) void** vmc_get_basic_funcs_ptr() {return (void**)&basic_funcs;}

int main(int argc, char const *argv[]){
    //#define DEBUG//#define DEBUG
    #ifdef DEBUG
        //argv[1] = "debug_file_name.bin";//argv[1] = "debug_file_name.bin";
    #else
        if (argc != 2) return ERROR_NOT_ENOUGH_ARGUMENTS;
    #endif
    file = fopen(argv[1], "rb");if (!file) return ERROR_FILE_OPEN;    
    crt = malloc(sizeof(int)*2);if (!crt) return ERROR_MEMORY_ALLOCATION;
    if (fread(crt, sizeof(int)*2, 1, file) != 1) return ERROR_FILE_READ;
    if (*(int*)crt == -1) {*(int*)crt = *((int*)crt + 1);} else {
        fseek(file, 0, SEEK_END);
        *(int*)crt = ftell(file);
        fseek(file, 0, SEEK_SET);
    }
    bse = crt = realloc(crt, *(int*)crt);if (!crt) return ERROR_MEMORY_REALLOCATION;
    if (fread(crt, *(int*)crt, 1, file) != 1) return ERROR_FILE_READ;
    imp = *(int*)crt;
    while (1) funcs[imp]();
}

// gcc vm.c -o vm.exe
