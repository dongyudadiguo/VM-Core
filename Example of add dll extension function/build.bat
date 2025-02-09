gcc loadfuncs.c -c -o loadfuncs.o
gcc dll_name.c loadfuncs.o -shared -o dll_name.dll
del *.o
