
#ifdef INCLUDE_LOADFUNCS
    typedef char** (*get_bse_ptr_func)();
    typedef char** (*get_crt_ptr_func)();
    typedef char** (*get_sdio_ptr_func)();
    typedef char** (*get_buff_ptr_func)();
    typedef FILE** (*get_f_ptr_func)();
    typedef void*** (*get_funcs_ptr_func)();
    typedef int* (*get_funcs_size_ptr_func)();
    typedef int* (*get_end_ptr_func)();
    typedef void** (*get_basic_funcs_ptr_func)();

    extern get_bse_ptr_func get_bse_ptr;
    extern get_crt_ptr_func get_crt_ptr;
    extern get_sdio_ptr_func get_sdio_pt;
    extern get_buff_ptr_func get_buff_pt;
    extern get_f_ptr_func get_f_ptr;
    extern get_funcs_ptr_func get_funcs_ptr;
    extern get_funcs_size_ptr_func get_funcs_size_ptr;
    extern get_end_ptr_func get_end_ptr;
    extern get_basic_funcs_ptr_func get_basic_funcs_ptr;

    extern char **crt_ptr, **bse_ptr, **sdio_ptr, **buff_ptr;
    extern FILE **file_ptr;
    extern void ***funcs_ptr;
    extern void **basic_funcs_ptr;
    extern int *end_ptr, *funcs_size_ptr;

    extern char *crt, *bse, *sdio, *buff;
    extern FILE *file;
    extern void **funcs;
    extern void *basic_funcs;
    extern int end, funcs_size; 
#else
    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>
    #include <windows.h>

    extern void (*new_funcs[])();
    extern int new_funcs_size;

    typedef char** (*get_bse_ptr_func)();
    typedef char** (*get_crt_ptr_func)();
    typedef char** (*get_sdio_ptr_func)();
    typedef char** (*get_buff_ptr_func)();
    typedef FILE** (*get_f_ptr_func)();
    typedef void*** (*get_funcs_ptr_func)();
    typedef int* (*get_funcs_size_ptr_func)();
    typedef int* (*get_end_ptr_func)();
    typedef void** (*get_basic_funcs_ptr_func)();

    static get_bse_ptr_func get_bse_ptr = NULL;
    static get_crt_ptr_func get_crt_ptr = NULL;
    static get_sdio_ptr_func get_sdio_ptr = NULL;
    static get_buff_ptr_func get_buff_ptr = NULL;
    static get_f_ptr_func get_f_ptr = NULL;
    static get_funcs_ptr_func get_funcs_ptr = NULL;
    static get_funcs_size_ptr_func get_funcs_size_ptr = NULL;
    static get_end_ptr_func get_end_ptr = NULL;
    static get_basic_funcs_ptr_func get_basic_funcs_ptr = NULL;

    char **crt_ptr, **bse_ptr, **sdio_ptr, **buff_ptr;
    FILE **file_ptr;
    void ***funcs_ptr;
    void **basic_funcs_ptr;
    int *end_ptr, *funcs_size_ptr;

    char *crt, *bse, *sdio, *buff;
    FILE *file;
    void **funcs;
    void *basic_funcs;
    int end, funcs_size; 

    void initialize_globals() {
        crt_ptr = get_crt_ptr();
        crt = *crt_ptr;
        bse_ptr = get_bse_ptr();
        bse = *bse_ptr;
        sdio_ptr = get_sdio_ptr();
        sdio = *sdio_ptr;
        buff_ptr = get_buff_ptr();
        buff = *buff_ptr;
        file_ptr = get_f_ptr();
        file = *file_ptr;
        funcs_ptr = get_funcs_ptr();
        funcs = *funcs_ptr;
        funcs_size_ptr = get_funcs_size_ptr();
        funcs_size = *funcs_size_ptr;
        end_ptr = get_end_ptr();
        end = *end_ptr;
        basic_funcs_ptr = get_basic_funcs_ptr();
        basic_funcs = *basic_funcs_ptr;
    }

    __declspec(dllexport) void loadfuncs() {
        // 获取主程序句柄并初始化函数指针
        HMODULE hModule = GetModuleHandle(NULL);
        
        get_bse_ptr = (get_bse_ptr_func)GetProcAddress(hModule, "vmc_get_bse_ptr");
        get_crt_ptr = (get_crt_ptr_func)GetProcAddress(hModule, "vmc_get_crt_ptr");
        get_sdio_ptr = (get_sdio_ptr_func)GetProcAddress(hModule, "vmc_get_sdio_ptr");
        get_buff_ptr = (get_buff_ptr_func)GetProcAddress(hModule, "vmc_get_buff_ptr");
        get_f_ptr = (get_f_ptr_func)GetProcAddress(hModule, "vmc_get_f_ptr");
        get_funcs_ptr = (get_funcs_ptr_func)GetProcAddress(hModule, "vmc_get_funcs_ptr");
        get_funcs_size_ptr = (get_funcs_size_ptr_func)GetProcAddress(hModule, "vmc_get_funcs_size_ptr");
        get_end_ptr = (get_end_ptr_func)GetProcAddress(hModule, "vmc_get_end_ptr");
        get_basic_funcs_ptr = (get_basic_funcs_ptr_func)GetProcAddress(hModule, "vmc_get_basic_funcs_ptr");

        initialize_globals();
        
        *funcs_ptr = realloc(*funcs_ptr, (*funcs_size_ptr + new_funcs_size) * sizeof(void*));
        
        memcpy(*(char***)funcs_ptr + *funcs_size_ptr - 1, new_funcs, new_funcs_size * sizeof(void*));
        *funcs_size_ptr += new_funcs_size - 1;
        *crt_ptr += 1*sizeof(int);
    }
#endif
