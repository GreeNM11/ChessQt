start cmd.exe /C "C:\Qt\6.9.0\mingw_64\bin\qtenv2.bat & cd /d C:\Documents\Coding_Projects\GitHub\ChessQt\build\ChessQt-debug & start ChessQt.exe --server"

timeout /t 0.5 >nul

start cmd.exe /C "C:\Qt\6.9.0\mingw_64\bin\qtenv2.bat & cd /d C:\Documents\Coding_Projects\GitHub\ChessQt\build\ChessQt-debug & start ChessQt.exe --clientL"

timeout /t 0.5 >nul

start cmd.exe /C "C:\Qt\6.9.0\mingw_64\bin\qtenv2.bat & cd /d C:\Documents\Coding_Projects\GitHub\ChessQt\build\ChessQt-debug & start ChessQt.exe --clientL"


