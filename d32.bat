@echo off
setlocal
set PATH=C:\D\dmd2\windows\bin;%PATH%
call vc32.bat dub.exe %* --arch=x86_mscoff
endlocal
