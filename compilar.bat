@echo off
echo Compilando Matrix Blocker...
gcc -o matrix_blocker.exe blocker_simples.c -luser32 -lkernel32
if %errorlevel% == 0 (
    echo Compilacao concluida com sucesso!
    echo Execute matrix_blocker.exe como administrador
) else (
    echo Erro na compilacao!
)
pause
