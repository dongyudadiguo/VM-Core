echo A ICON "icon.ico">ico.rc
windres ico.rc ico.o
gcc *.c ico.o -o vm.exe
del ico.rc ico.o
