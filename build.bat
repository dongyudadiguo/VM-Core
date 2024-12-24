@echo off
choice /m "add icon?"
if %errorlevel%==1 (
	echo A ICON "icon.ico">ico.rc
	windres ico.rc ico.o
	gcc *.c ico.o -o tinyvm.exe
	del ico.rc ico.o
)
if %errorlevel%==2 (
	gcc *.c -o tinyvm.exe
)
tinyvm.exe
pause>nul
