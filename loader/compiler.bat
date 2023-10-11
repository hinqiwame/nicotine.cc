@echo off
echo [~] compiling nicotine loader...
pyinstaller --onefile --icon=assets/icon.ico nicotine.py
rd /s /q build
del /q nicotine.spec
if not exist build mkdir build
move /y dist\*.* build
rd /s /q dist
echo [+] done.
pause
