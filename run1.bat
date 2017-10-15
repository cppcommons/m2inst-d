call mg32.bat qmake m2boot.pro -spec win32-g++ "CONFIG+=release"
call mg32.bat mingw32-make -f Makefile.Release
release\m2boot.exe

call d32.bat build --build=release
copy C:\D\dmd2\windows\bin\libcurl.dll .

set MSYS2_NAME=ms2inst
set MSYS2_BITS=32
set MSYS2_PKGS=diffutils,procps,psmisc
set MSYS2_PKGS=%MSYS2_PKGS%,tmux-git &:: THIS IS TMUX
set MSYS2_PKGS=%MSYS2_PKGS%,vim      &:: THIS IS VIM
set MSYS2_PKGS=%MSYS2_PKGS%,         &:: THIS IS EMPTY
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

ms2run1.exe

pause
