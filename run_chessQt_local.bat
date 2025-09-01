
start cmd.exe /C "C:\Qt\6.9.0\mingw_64\bin\qtenv2.bat & cd /d C:\Documents\Coding_Projects\GitHub\ChessQt\build\Desktop_Qt_6_9_0_MinGW_64_bit-MinSizeRel & start ChessQt.exe --server"

timeout /t 0.5 >nul

start cmd.exe /C "C:\Qt\6.9.0\mingw_64\bin\qtenv2.bat & cd /d C:\Documents\Coding_Projects\GitHub\ChessQt\build\Desktop_Qt_6_9_0_MinGW_64_bit-MinSizeRel & start ChessQt.exe --clientL"

timeout /t 0.5 >nul

start cmd.exe /C "C:\Qt\6.9.0\mingw_64\bin\qtenv2.bat & cd /d C:\Documents\Coding_Projects\GitHub\ChessQt\build\Desktop_Qt_6_9_0_MinGW_64_bit-MinSizeRel & start ChessQt.exe --clientL"


