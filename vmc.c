#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_LIBRARY_LOAD -1
#define ERROR_FUNCTION_LOAD -2
#define ERROR_FILE_OPEN -3
#define ERROR_FILE_READ -4
#define ERROR_FILE_SEEK -5
#define ERROR_FILE_FTELL -6
#define ERROR_NOT_ENOUGH_ARGUMENTS -7
#define ERROR_MEMORY_ALLOCATION -8
#define ERROR_MEMORY_REALLOCATION -9

void SFT(void);
void IJMP(void);
void NJMP(void);
void MOV(void);
void EMOV(void);
void WMOV(void);
void FMOV(void);
void OVER(void);
void SDIO(void);
void BSE(void);
void CRT(void);
void BUFSET(void);
void DLL(void);
void OUTPUT(void);

char *crt, *bse, *sdio, *buff;
FILE *file;
#define basic_instruction SFT,IJMP,NJMP,MOV,EMOV,WMOV,FMOV,OVER,SDIO,BSE,CRT,DLL,BUFSET,OUTPUT//,newfunc
void (*basic_funcs[])() = {basic_instruction};
void (**funcs)() = basic_funcs;
int end = 0, funcs_size = sizeof(basic_funcs) / sizeof(basic_funcs[0]);

void SFT(void){
    crt = bse + *((int*)crt + 1);
}
void IJMP(void){
    if(*(char*)(bse + *((int*)crt + 1)) == 1){
        crt = bse + *((int*)crt + 2);
    }else{
        crt += 2*sizeof(int)+1*sizeof(int);
    }
}
void NJMP(void){
    if(*(char*)(bse + *((int*)crt + 1)) == 0){
        crt = bse + *((int*)crt + 2);
    }else{
        crt += 2*sizeof(int)+1*sizeof(int);
    }
}
void MOV(void){
    memmove(bse + *((int*)crt + 1), bse + *((int*)crt + 2), *((int*)crt + 3));
    crt += 3*sizeof(int)+1*sizeof(int);
}
void EMOV(void){
    memmove(bse + *((int*)crt + 1), *(char**)((int*)crt + 2), *((int*)crt + 3));
    crt += 3*sizeof(int)+1*sizeof(int);
}
void WMOV(void){
    memmove(*(char**)((int*)crt + 1), bse + *((int*)crt + 2), *((int*)crt + 3));
    crt += 3*sizeof(int)+1*sizeof(int);
}
void FMOV(void){
    memmove(*(char**)((int*)crt + 1), *(char**)((int*)crt + 2), *((int*)crt + 3));
    crt += 3*sizeof(int)+1*sizeof(int);
}
void OVER(void){
    end = *((int*)crt + 1);
}
void SDIO(void){
    *(char**)((int*)crt + 1) = sdio;
    crt += 1*sizeof(int)+1*sizeof(int);
}
void BSE(void){
    *(char**)((int*)crt + 1) = bse;
    crt += 1*sizeof(int)+1*sizeof(int);
}
void CRT(void){
    *(char**)((int*)crt + 1) = crt;
    crt += 1*sizeof(int)+1*sizeof(int);
}
void BUFSET(void){
    buff = bse + *((int*)crt + 1);
    crt += 1*sizeof(int)+1*sizeof(int);
}
void DLL(void){
    HINSTANCE hLib = *(HINSTANCE*)(buff) = LoadLibrary(crt + sizeof(int));
    if (hLib == NULL) {
        *(int*)(buff) = ERROR_LIBRARY_LOAD;
        return;
    }
    crt += sizeof(int);
    while(*crt++ != 0);
    do{
        funcs_size += 1;
        if (funcs == basic_funcs){
            funcs = malloc((funcs_size) * sizeof(void(*)()));
            if (funcs == NULL) {
                *(int*)(buff) = ERROR_MEMORY_ALLOCATION;
                return;
            }
            memcpy(funcs, basic_funcs, (funcs_size - 1) * sizeof(void(*)()));
        }else{
            funcs = realloc(funcs, (funcs_size) * sizeof(void(*)()));
            if (funcs == NULL) {
                *(int*)(buff) = ERROR_MEMORY_REALLOCATION;
                return;
            }
        }
        FARPROC proc = GetProcAddress(hLib, crt);
        if(proc == NULL){
            *(int*)(buff) = ERROR_FUNCTION_LOAD;
            return;
        }else{
            funcs[funcs_size - 1] = (void(*)(void))proc;
        }
        while(*crt++ != 0);
    }while(*(crt) != 0);
    crt += 1;
}
void OUTPUT(void){
    printf("%s", bse + *((int*)crt + 1));
    crt += 1*sizeof(int)+1*sizeof(int);
}
// void newfunc(void){
//     printf("is newfunc there");
//     crt += 1*sizeof(int);
// }

__declspec(dllexport) char** vmc_get_bse_ptr() {
   return &bse;
}

__declspec(dllexport) char** vmc_get_crt_ptr() {
   return &crt;
}

__declspec(dllexport) char** vmc_get_sdio_ptr() {
   return &sdio;
}

__declspec(dllexport) char** vmc_get_buff_ptr() {
   return &buff;
}

__declspec(dllexport) FILE** vmc_get_f_ptr() {
   return &file;
}

__declspec(dllexport) void*** vmc_get_funcs_ptr() {
   return (void***)&funcs;
}

__declspec(dllexport) int* vmc_get_funcs_size_ptr() {
   return &funcs_size;
}

__declspec(dllexport) int* vmc_get_end_ptr() {
   return &end;
}

__declspec(dllexport) void** vmc_get_basic_funcs_ptr() {
   return (void**)&basic_funcs;
}

int main(int argc, char const *argv[]){
    
    #ifdef DEBUG
        //argv[1] = "debug_file_name.bin";
    #else
        if (argc != 2) return ERROR_NOT_ENOUGH_ARGUMENTS;
    #endif

    if ((file = fopen(argv[1], "rb")) == NULL) return ERROR_FILE_OPEN;
    if ((bse = crt = malloc(1 * sizeof(int))) == NULL) return ERROR_MEMORY_ALLOCATION;
    if (fread(crt, 1 * sizeof(int), 1, file) != 1) return ERROR_FILE_READ;
    if (*(int*)crt == -1){
        if(fread(crt, 1 * sizeof(int), 1, file) != 1) return ERROR_FILE_READ;
        if ((bse = crt = realloc(crt, *(int*)crt)) == NULL) return ERROR_MEMORY_REALLOCATION;
        if(fread(crt, *(int*)crt, 1, file) != 1) return ERROR_FILE_READ;
    }else{
        if(fseek(file, 0, SEEK_END) != 0) return ERROR_FILE_SEEK;
        int file_size = ftell(file);
        if(file_size == -1) return ERROR_FILE_FTELL;
        if(fseek(file, 0, SEEK_SET) != 0) return ERROR_FILE_SEEK;
        if ((bse = crt = realloc(crt, file_size)) == NULL) return ERROR_MEMORY_REALLOCATION;
        if(fread(crt, file_size, 1, file) != 1) return ERROR_FILE_READ;
    }
    while (end == 0) {funcs[*(int*)crt]();}
    return end;
}
