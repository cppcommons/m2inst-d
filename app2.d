// from http://forum.dlang.org/post/otuvjlaoivpubftxdhxt@forum.dlang.org
// #define CSIDL_PROFILE 0x0028
import core.sys.windows.windows; // C:\D\dmd2\src\druntime\src\core\sys\windows
import core.sys.windows.shlobj: CSIDL_PROFILE;

import core.sys.windows.winhttp; // C:\D\dmd2\src\druntime\src\core\sys\windows


void main() {
	import std.stdio;
	{
		import core.stdc.wchar_: wcslen;
		import std.conv: to;
		import std.string : toStringz;
		import std.utf: toUTF16z;
		string url = "https://raw.githubusercontent.com/cyginst/ms2inst-v1/master/binaries/msys2-i686-20161025.7z";
		URL_COMPONENTS urlComponents;

		assert(urlComponents.dwStructSize == 0);
		urlComponents.dwStructSize = urlComponents.sizeof;

		const wchar * urlZ = to!(wstring)(url).toUTF16z;
		{ import core.stdc.stdio; printf("%ls\n", urlZ); }
		writeln( wcslen(urlZ) );

		wchar[] hostNameZ;
		hostNameZ.length = wcslen(urlZ)+1;
		urlComponents.lpszHostName = cast(wchar *)&hostNameZ[0];
		urlComponents.dwHostNameLength = hostNameZ.length;
		writeln(hostNameZ.length);

		wchar[] urlPathZ;
		urlPathZ.length = wcslen(urlZ)+1;
		urlComponents.lpszUrlPath = cast(wchar *)&urlPathZ[0];
		urlComponents.dwUrlPathLength = urlPathZ.length;
		writeln(urlPathZ.length);

		if (!WinHttpCrackUrl(urlZ, wcslen(urlZ), 0, &urlComponents)) {
		    writeln("1");
			return;
		}
	    writeln("2");
		//writeln(hostNameZ);
		writeln(hostNameZ[0 .. wcslen(cast(wchar *)hostNameZ)]);
		writeln(urlPathZ[0 .. wcslen(cast(wchar *)urlPathZ)]);
		//writeln(urlPathZ);
	}
}

