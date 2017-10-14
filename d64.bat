@echo off
setlocal
set PATH=C:\D\dmd2\windows\bin;%PATH%
call vc64.bat dub.exe %* --arch=x86_64
endlocal
