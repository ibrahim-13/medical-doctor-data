@echo off
cls
echo "Stating BlockchainServer..."
start ./BlockchainServerRelease/BlockchainServer.exe
echo "Starting nginx server..."
cd "./nginx-1.12.2"
call "./start_nginx_server.bat"
exit