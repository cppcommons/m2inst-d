dir winhttp-dm32.lib
pause
C:\D\dmd2\windows\bin\dmd.exe -I=lib app.d lib/pegged-dm32.lib winhttp-dm32.lib
setlocal
call "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/vcvarsall.bat" x86
::C:\D\dmd2\windows\bin\dmd.exe -m32mscoff -I=lib app2.d lib/pegged-ms32.lib winhttp.lib
endlocal

if %errorlevel% neq 0 (
  echo エラー発生：バッチファイルは異常終了しました。「errorlevel」は「%errorlevel%」です。
  pause
  exit /b
)
if not exist libcurl.dll copy C:\D\dmd2\windows\bin\libcurl.dll .

set MSYS2_NAME=ms2inst
set MSYS2_BITS=32
set MSYS2_PKGS=test;diffutils;procps psmisc , (this is comment)
set MSYS2_PKGS=%MSYS2_PKGS%,2test;48diffutils;p33rocps msys/psmisc , (this is comment)
set MSYS2_PKGS=%MSYS2_PKGS%,(tmux-git) &:: THIS IS TMUX
set MSYS2_PKGS=%MSYS2_PKGS%,vim      &:: THIS IS VIM
set MSYS2_PKGS=%MSYS2_PKGS%,         &:: THIS IS EMPTY
::set MSYS2_PKGS=        =                                      
::set MSYS2_PKGS=                                              
set MSYS2_USE_MINGW32=1
set MSYS2_USE_MINGW64=1
set MSYS2_USE_MSYS=1
set DT_ICONS=1
::set MSYS2_HOME=.
::set MSYS2_ASIS=1

set MSYS2_DEBUG=0
set MSYS2_FONT=MS Gothic
set MSYS2_FONT_HEIGHT=12
set MSYS2_CURSOR_TYPE=block
set MSYS2_CONFIRM_EXIT=no

::set

app2.exe

pause