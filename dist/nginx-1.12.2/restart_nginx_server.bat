@echo off
cls
taskkill /f /t /im "nginx.exe"
start nginx.exe
exit