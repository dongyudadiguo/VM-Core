set source=you_lib_name

gcc %source%.c -c -o %source%.o
gcc loadfuncs.c -c -o loadfuncs.o

gcc %source%.o loadfuncs.o -shared -o %source%.dll

del %source%.o
del loadfuncs.o