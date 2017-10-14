import std.net.curl, std.stdio;

int main() {
// Return a char[] containing the content specified by a URL
//auto content = get("dlang.org");
auto content = get("https://raw.githubusercontent.com/cyginst/cyginst-v1/master/cyginst.bat");

writeln(content.length);
return 0;
}