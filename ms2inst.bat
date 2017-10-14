@if(0)==(0) echo off
:: URL: https://github.com/cyginst/ms2inst-v1/blob/master/ms2inst.bat
setlocal ENABLEDELAYEDEXPANSION

if "%1"=="SUBPROC" goto skip_init

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

:skip_init


set SCRIPT=%~0
for /f "delims=\ tokens=*" %%z in ("%SCRIPT%") do (set SCRIPT_CURRENT_DIR=%%~dpz)

if "%MSYS2_DEBUG%"=="1" echo on
set MSYS2_SETUP=
if "%MSYS2_BITS%"=="32" (
    set MSYS2_SETUP=msys2-i686-20161025.7z
) else if "%MSYS2_BITS%"=="64" (
    set MSYS2_SETUP=msys2-x86_64-20161025.7z
) else (
    echo MSYS2_BITS must be 32 or 64. [Current MSYS2_BITS: %MSYS2_BITS%] Aborting!
    if not "%1"=="SUBPROC" pause
    exit /b
)
if not exist .binaries mkdir .binaries
if not exist wget.exe call :dl_from_url wget.exe https://raw.githubusercontent.com/cyginst/ms2inst-v1/master/binaries/wget.exe
.binaries\wget -nc --no-check-certificate -P .binaries https://raw.githubusercontent.com/cyginst/ms2inst-v1/master/binaries/7z.exe
.binaries\wget -nc --no-check-certificate -P .binaries https://raw.githubusercontent.com/cyginst/ms2inst-v1/master/binaries/7z.dll
.binaries\wget -nc --no-check-certificate -P .binaries https://raw.githubusercontent.com/cyginst/ms2inst-v1/master/binaries/%MSYS2_SETUP%
set MSYS2_ROOT=%SCRIPT_CURRENT_DIR%%MSYS2_NAME%.m%MSYS2_BITS%
if not exist "%MSYS2_ROOT%" (
    if exist "%MSYS2_ROOT%.tmp" rmdir /s /q "%MSYS2_ROOT%.tmp"
    .binaries\7z.exe x -y -o"%MSYS2_ROOT%.tmp" ".binaries\%MSYS2_SETUP%" && move "%MSYS2_ROOT%.tmp" "%MSYS2_ROOT%"
)
set HOME=%MSYS2_ROOT%
set cmd="%MSYS2_ROOT%\usr\bin\bash.exe" -l -c "pacman --noconfirm -Fy"
echo %cmd%
%cmd%
if not "%MSYS2_PKGS%"=="" (
  for %%a in ("%MSYS2_PKGS:,=" "%") do (
    set MSYS2_PKG=%%~a
    call :trim !MSYS2_PKG! MSYS2_PKG
    if not "!MSYS2_PKG!"=="" (
      echo [!MSYS2_PKG!]
      set cmd="%MSYS2_ROOT%\usr\bin\bash.exe" -l -c "pacman --noconfirm --needed -S !MSYS2_PKG!"
      echo !cmd!
      !cmd!
    )
  )
)
cscript.exe //nologo //E:JScript "%~f0"

echo Installation for %MSYS2_NAME% finished!

endlocal
if not "%1"=="SUBPROC" pause
exit /b
goto :EOF

:dl_from_url
if not exist .binaries mkdir .binaries
if not exist "%SCRIPT_CURRENT_DIR%.binaries\%1" bitsadmin /TRANSFER "%1" "%2" "%SCRIPT_CURRENT_DIR%.binaries\%1"
exit /b

:trim
set %2=%1
exit /b

@end

  if (!String.prototype.format) {
    String.prototype.format = function() {
      var args = arguments;
      return this.replace(/{(\d+)}/g, function(match, number) { 
        return typeof args[number] != 'undefined'
          ? args[number]
          : match
        ;
      });
    };
  }

  if (!String.prototype.startsWith) {
    String.prototype.startsWith = function(prefix) {
      return (this.lastIndexOf(prefix, 0) === 0);
    };
  }

  if (!String.prototype.endsWith) {
    String.prototype.endsWith = function(suffix) {
      var sub = this.length - suffix.length;
      return (sub >= 0) && (this.lastIndexOf(suffix) === sub);
    };
  }

  if (!String.prototype.removeFirst) {
    String.prototype.removeFirst = function(prefix) {
      if (this.lastIndexOf(prefix, 0) === 0) {
        return this.substr(prefix.length);
      } else {
        return this;
      }
    };
  }

  if (!String.prototype.removeLast) {
    String.prototype.removeLast = function(suffix) {
      var sub = this.length - suffix.length;
      if (sub >= 0 && this.lastIndexOf(suffix) === sub) {
        return this.substr(0, this.lastIndexOf(suffix));
      } else {
        return this;
      }
    };
  }

  var env = new ActiveXObject("WScript.Shell").Environment("PROCESS");

  var MSYS2_OPTS = {
    debug:       (env.item("MSYS2_DEBUG") == "1"),
    root:         env.item("MSYS2_ROOT"),
    name:         env.item("MSYS2_NAME"),
    bits:         env.item("MSYS2_BITS"),
    use_mingw32: (env.item("MSYS2_USE_MINGW32") == "1"),
    use_mingw64: (env.item("MSYS2_USE_MINGW64") == "1"),
    use_msys:    (env.item("MSYS2_USE_MSYS") == "1"),
    dt_icons:    (env.item("DT_ICONS") == "1"),
    home:         env.item("MSYS2_HOME").replace(/^\s+|\s+$/g, ''), /*trim()*/
    asis:        (env.item("MSYS2_ASIS") == "1"),
    font:         env.item("MSYS2_FONT"),
    font_height:  env.item("MSYS2_FONT_HEIGHT"),
    cursor_type:  env.item("MSYS2_CURSOR_TYPE"),
    confirm_exit: env.item("MSYS2_CONFIRM_EXIT")
  };

  postCygwinInstall(MSYS2_OPTS);

  WScript.Quit();

  function echo(msg) {
    WScript.Echo(msg);
  }
  function msgbox(msg) {
    var shell = new ActiveXObject("WScript.Shell");
    shell.Popup(msg, 0, "Cygwin Install", 0);
  }

  function postCygwinInstall(opts) {

    var StreamTypeEnum    = { adTypeBinary: 1, adTypeText: 2 };
    var LineSeparatorEnum = { adLF: 10, adCR: 13, adCRLF: -1 };
    var StreamReadEnum    = { adReadAll: -1, adReadLine: -2 };
    var StreamWriteEnum   = { adWriteChar: 0, adWriteLine: 1 };
    var SaveOptionsEnum   = { adSaveCreateNotExist: 1, adSaveCreateOverWrite: 2 };

    function createShorcut(dir, name, icon, target, args) {
      var shell = new ActiveXObject("WScript.Shell");
      var sc = shell.CreateShortcut(dir + "\\" + name + ".lnk");
      sc.IconLocation = icon;
      sc.TargetPath = target;
      sc.Arguments = args;
      sc.WorkingDirectory = "";
      sc.Save();
    }

    function createShorcut2(used, dir, name, icon, target, args) {
      var scPath = dir + "\\" + name + ".lnk";
      if (!used) {
        deleteFile(scPath);
        return;
      }
      var shell = new ActiveXObject("WScript.Shell");
      var sc = shell.CreateShortcut(scPath);
      sc.IconLocation = icon;
      sc.TargetPath = target;
      sc.Arguments = args;
      sc.WorkingDirectory = "";
      sc.Save();
    }

    function writeTextToFile_Utf8_NoBOM(path, text) {
      var stream = new ActiveXObject("ADODB.Stream");
      stream.Type = StreamTypeEnum.adTypeText;
      stream.Charset = "utf-8";
      stream.Open();
      stream.WriteText(text);
      stream.Position = 0
      stream.Type = StreamTypeEnum.adTypeBinary;
      stream.Position = 3
      var buf = stream.Read();
      stream.Position = 0
      stream.Write(buf);
      stream.SetEOS();
      stream.SaveToFile(path, SaveOptionsEnum.adSaveCreateOverWrite);
      stream.Close();
    }

    function deleteFile(path) {
      var fso = new ActiveXObject("Scripting.FileSystemObject");
      try {
        fso.DeleteFile(path, true);
      } catch(e) {
      }
    }

    function editConfigFile(path, lineStart, lineAfter, override) {
      if (lineAfter == null) override = true;
      var txt = "";
      var found = false;
      var match = false;
      try {
        var stream1 = new ActiveXObject("ADODB.Stream");
        stream1.Type = StreamTypeEnum.adTypeText;
        stream1.Charset = "utf-8";
        stream1.LineSeparator = LineSeparatorEnum.adLF;
        stream1.Open();
        stream1.LoadFromFile(path);
        while (!stream1.EOS) {
          var line = stream1.ReadText(StreamReadEnum.adReadLine);
            if (line.startsWith(lineStart)) {
              found = true;
              if (!override) break;
              if (lineAfter == null) continue;
              if (line == lineAfter) {
                match = true;
                break;
              }
              line = lineAfter;
            }
            txt += line + "\n";
        }
        if (!found && lineAfter != null) txt += lineAfter + "\n";
        stream1.Close();
      } catch(e) {
        if (lineAfter != null) txt = lineAfter + "\n";
      }
      if (match) return;
      if (found && !override) return;
      if (txt == "")
        deleteFile(path)
      else
        writeTextToFile_Utf8_NoBOM(path, txt);
    }

    function defaultSetting(path, lineStart, lineAfter) { editConfigFile(path, lineStart, lineAfter, false); }

    function replaceSetting(path, lineStart, lineAfter) { editConfigFile(path, lineStart, lineAfter, true); }

    var fso = new ActiveXObject("Scripting.FileSystemObject");
    var shell = new ActiveXObject("WScript.Shell");

    var profilePath = opts.root + "\\etc\\profile";
    if (opts.home == "") {
      replaceSetting(profilePath, "MSYS2_PATH=", "MSYS2_PATH=\"/usr/local/bin:/usr/bin:/bin\"");
    } else {
      var absoluteHome = opts.home.endsWith("$");
      opts.home = opts.home.removeLast("$");
      opts.home = fso.GetAbsolutePathName(opts.home).replace(new RegExp("\\\\", "g"), "/");
      opts.home = opts.home.removeLast("/");
      var unixHome = "`/usr/bin/cygpath -u {0}`".format(opts.home);
      replaceSetting(profilePath, "MSYS2_PATH=", "MSYS2_PATH=\"/usr/local/bin:/usr/bin:/bin\";HOME=" + unixHome + (absoluteHome ? "" : "/$USER"));
    }

    var bashrcPath = opts.root + "\\etc\\bash.bashrc";
    var bashrcOrig = bashrcPath + ".orig";
    if (fso.FileExists(bashrcPath)) {
      if (!fso.FileExists(bashrcOrig)) {
        fso.CopyFile(bashrcPath, bashrcOrig, true);
        defaultSetting(bashrcPath, "[[ -n \"${MSYS2_PS1}\" ]] && export PS1=", null);
        defaultSetting(bashrcPath, "[[ $(declare -p PS1 2>/dev/null | cut -c 1-11) = 'declare -x ' ]]", null);
            var ps1 = ("if [ \"x$INSIDE_EMACS\" != \"x\" ]; then export PS1='\\[\\e[35m\\]\\u@{0} $MSYSTEM({1}bit) \\w\\[\\e[0m\\]\\n\\$ '; "
                       + "else export PS1='\\[\\e]0;$MSYSTEM ({1}bit) @{0} \\w\\a\\]\\n\\[\\e[32m\\]\\u@{0} \\[\\e[35m\\]$MSYSTEM({1}bit)\\[\\e[0m\\] \\[\\e[33m\\]\\w\\[\\e[0m\\]\\n\\$ '; fi")
                      .format(opts.name, opts.bits);
        replaceSetting(bashrcPath, "  export PS1=", ps1);
      }
    }

    var desktopPath = shell.SpecialFolders("Desktop");
    if (opts.debug) echo("desktopPath=" + desktopPath);

    var minttyPath = opts.root + "\\usr\\bin\\mintty.exe";
    if (!fso.FileExists(minttyPath)) return;

    var minttyrcPath = opts.root + "\\etc\\minttyrc";
    if (opts.font != "")         defaultSetting(minttyrcPath, "Font=",        "Font={0}".format(opts.font));
    if (opts.font_height != "")  defaultSetting(minttyrcPath, "FontHeight=",  "FontHeight={0}".format(opts.font_height));
    if (opts.cursor_type != "")  defaultSetting(minttyrcPath, "CursorType=",  "CursorType={0}".format(opts.cursor_type));
    if (opts.confirm_exit != "") defaultSetting(minttyrcPath, "ConfirmExit=", "ConfirmExit={0}".format(opts.confirm_exit));

    var minttyCommon = "--window max";

    var name, icon, target, args;
    function scMingwShell(msystem, used) {
      name = "{0} Shell @{1} ({2}bit)".format(msystem, opts.name, opts.bits);
      icon = opts.root + "\\msys2.ico";
      target = minttyPath;
      args = "{1} -i /msys2.ico /usr/bin/env MSYSTEM={0} /usr/bin/bash -l -i".format(msystem, minttyCommon);
      createShorcut2(used, opts.root, name, icon, target, args);
      if (opts.dt_icons) createShorcut2(used, desktopPath, name, icon, target, args);
    }
    scMingwShell("MINGW32", opts.use_mingw32);
    scMingwShell("MINGW64", opts.use_mingw64);
    scMingwShell("MSYS"   , opts.use_msys);

    var vimPath = opts.root + "\\usr\\bin\\vim.exe";
    if (fso.FileExists(vimPath)) {
      var viText = "#!/usr/bin/bash\n/usr/bin/vim $*";
      writeTextToFile_Utf8_NoBOM(opts.root + "\\usr\\bin\\vi", viText);
    }

    var tmuxPath = opts.root + "\\usr\\bin\\tmux.exe";
    if (fso.FileExists(tmuxPath)) {
      var tmuxConfPath = opts.root + "\\etc\\tmux.conf";
      
      replaceSetting(tmuxConfPath, "set-option -ga update-environment ",
                opts.asis ? null : "set-option -ga update-environment ' MSYSTEM'");
      replaceSetting(tmuxConfPath, "bind -n S-up ",
                opts.asis ? null : "bind -n S-up select-pane -U \\; display-panes");
      replaceSetting(tmuxConfPath, "bind -n S-down ",
                opts.asis ? null : "bind -n S-down select-pane -D \\; display-panes");
      replaceSetting(tmuxConfPath, "bind -n S-left ",
                opts.asis ? null : "bind -n S-left select-pane -L \\; display-panes");
      replaceSetting(tmuxConfPath, "bind -n S-right ",
                opts.asis ? null : "bind -n S-right select-pane -R \\; display-panes");
      replaceSetting(tmuxConfPath, "bind-key -n C-Up ",
                opts.asis ? null : "bind-key -n C-Up resize-pane -U \\; display-panes");
      replaceSetting(tmuxConfPath, "bind-key -n C-Down ",
                opts.asis ? null : "bind-key -n C-Down resize-pane -D \\; display-panes");
      replaceSetting(tmuxConfPath, "bind-key -n C-Left ",
                opts.asis ? null : "bind-key -n C-Left resize-pane -L \\; display-panes");
      replaceSetting(tmuxConfPath, "bind-key -n C-Right ",
                opts.asis ? null : "bind-key -n C-Right resize-pane -R \\; display-panes");
      var name, icon, target, args;
      function scTmux(msystem, used) {
        name = "Tmux {0} @{1} ({2}bit)".format(msystem, opts.name, opts.bits);
        icon = opts.root + "\\tmux.ico";
        target = minttyPath;
        args = "{1} -i /usr/bin/mintty.exe -t \"Tmux {0} @{2} ({3}bit)\" /usr/bin/env MSYSTEM={0} /usr/bin/bash -l -c  \"/usr/bin/tmux new-session\""
               .format(msystem, minttyCommon, opts.name, opts.bits);
        createShorcut2(used, opts.root, name, icon, target, args);
        if (opts.dt_icons) createShorcut2(used, desktopPath, name, icon, target, args);
      }
      scTmux("MINGW32", opts.use_mingw32);
      scTmux("MINGW64", opts.use_mingw64);
      scTmux("MSYS"   , opts.use_msys);
    }

    var emacsPath = opts.root + "\\usr\\bin\\emacs.exe";
    if (fso.FileExists(emacsPath)) {
      var name, icon, target, args;
      function scEmacs(msystem, used) {
        name = "Emacs {0} @{1} ({2}bit)".format(msystem, opts.name, opts.bits);
        icon = opts.root + "\\emacs.ico";
        target = minttyPath;
        args = "{1} -i /emacs.ico -t \"Emacs {0} @{2} ({3}bit)\" /usr/bin/env MSYSTEM={0} /usr/bin/bash -l -c  \"/usr/bin/emacs -nw --eval '(progn (shell) (delete-other-windows))'\""
               .format(msystem, minttyCommon, opts.name, opts.bits);
        createShorcut2(used, opts.root, name, icon, target, args);
        if (opts.dt_icons) createShorcut2(used, desktopPath, name, icon, target, args);
      }
      scEmacs("MINGW32", opts.use_mingw32);
      scEmacs("MINGW64", opts.use_mingw64);
      scEmacs("MSYS"   , opts.use_msys);
    }

    function editEmacsSiteStart(siteStartPath) {
      if (fso.FolderExists(fso.getParentFolderName(siteStartPath))) {
        //echo("EDIT: " + siteStartPath);
        defaultSetting(siteStartPath,
          "(setq frame-title-format ",
          "(setq frame-title-format (format \"[%%b] @{0} [%s] - Emacs ({1}bit)\" (getenv \"MSYSTEM\")))".format(opts.name, opts.bits));
        defaultSetting(siteStartPath,
          "(set-frame-parameter nil 'fullscreen ",
          opts.asis ? null : "(set-frame-parameter nil 'fullscreen 'maximized)");
        defaultSetting(siteStartPath,
          "(windmove-default-keybindings",
          opts.asis ? null : "(windmove-default-keybindings 'meta)");
        defaultSetting(siteStartPath, "(global-set-key (kbd \"\\e <up>\")",
                   opts.asis ? null : "(global-set-key (kbd \"\\e <up>\") 'windmove-up)");
        defaultSetting(siteStartPath, "(global-set-key (kbd \"\\e <down>\")",
                   opts.asis ? null : "(global-set-key (kbd \"\\e <down>\") 'windmove-down)");
        defaultSetting(siteStartPath, "(global-set-key (kbd \"\\e <left>\")",
                   opts.asis ? null : "(global-set-key (kbd \"\\e <left>\") 'windmove-left)");
        defaultSetting(siteStartPath, "(global-set-key (kbd \"\\e <right>\")",
                   opts.asis ? null : "(global-set-key (kbd \"\\e <right>\") 'windmove-right)");
        defaultSetting(siteStartPath, "(global-set-key (kbd \"\\C-x <kp-add>\")",
                   opts.asis ? null : "(global-set-key (kbd \"\\C-x <kp-add>\") 'balance-windows)");
      }
    }
    editEmacsSiteStart(opts.root + "\\usr\\share\\emacs\\site-lisp\\site-start.el");
    editEmacsSiteStart(opts.root + "\\mingw32\\share\\emacs\\site-lisp\\site-start.el");
    editEmacsSiteStart(opts.root + "\\mingw64\\share\\emacs\\site-lisp\\site-start.el");

  }

