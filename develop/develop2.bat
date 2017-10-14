@if(0)==(0) echo off
setlocal

REM --- EDIT THIS PART(START) ---
set MSYS2_NAME=develop2
set MSYS2_BITS=32
set MSYS2_PKGS=diffutils,procps,psmisc
set MSYS2_PKGS=%MSYS2_PKGS%,tmux-git               &:: THIS IS TMUX
set MSYS2_PKGS=%MSYS2_PKGS%,vim                    &:: THIS IS VIM
set MSYS2_PKGS=%MSYS2_PKGS%,emacs,glib2,libhogweed &:: THIS IS EMACS
set MSYS2_USE_MINGW32=1
::set MSYS2_USE_MINGW64=1
::set MSYS2_USE_MSYS=1
set DT_ICONS=1
set MSYS2_HOME=.
::set MSYS2_ASIS=1
REM --- EDIT THIS PART(END) ---

REM --- DEBUG/CUSTOMIZE(START) ---
set MSYS2_DEBUG=0
set MSYS2_FONT=MS Gothic
set MSYS2_FONT_HEIGHT=12
set MSYS2_CURSOR_TYPE=block
set MSYS2_CONFIRM_EXIT=no
REM --- DEBUG/CUSTOMIZE(END) ---

cscript.exe //nologo //E:JScript "%~f0"
call ms2inst.bat SUBPROC
endlocal
pause
exit /b
goto :EOF
@end
var fso = new ActiveXObject("Scripting.FileSystemObject");
var SCRIPT_CURRENT_DIR = fso.getParentFolderName(WScript.ScriptFullName);
var url = "https://raw.githubusercontent.com/cyginst/ms2inst-v1/master/ms2inst.bat";
var fileName = SCRIPT_CURRENT_DIR + "\\ms2inst.bat";
try {
  downloadFile(url, fileName);
} catch (e) {
  WScript.Echo("Could not download: ms2inst.bat");
}
WScript.Quit();
function downloadFile(url, fileName) {
  var StreamTypeEnum  = { adTypeBinary: 1, adTypeText: 2 };
  var SaveOptionsEnum = { adSaveCreateNotExist: 1, adSaveCreateOverWrite: 2 };
  var http = WScript.CreateObject("MSXML2.XMLHTTP");
  var strm = WScript.CreateObject("ADODB.Stream");
  http.Open("GET", url, false);
  http.Send();
  strm.Type = StreamTypeEnum.adTypeBinary;
  strm.Open();
  strm.Write(http.responseBody);
  strm.SaveToFile(fileName, SaveOptionsEnum.adSaveCreateOverWrite);
}

