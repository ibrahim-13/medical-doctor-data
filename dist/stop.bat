@echo off
cls
echo "Terminating BlockchainServer..."
taskkill /f /t /im "BlockchainServer.exe"
echo "Terminating nginx server..."
cd "./nginx-1.12.2"
call "./stop_nginx_server.bat"
exit