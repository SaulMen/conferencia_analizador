@echo off
echo Compilando QuestScript Backend con httplib...
g++ -std=c++17 src/Scanner.cpp src/Parser.cpp src/main.cpp -I ./lib -I ./src -o server.exe -lws2_32
if %errorlevel% neq 0 (
    echo Error en la compilacion.
    pause
    exit /b %errorlevel%
)
echo Compilacion exitosa. Ejecutando servidor...
server.exe