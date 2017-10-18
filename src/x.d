/* Converted to D from x.h by htod */
module x;
//C     #ifdef __cplusplus
//C     extern "C" {
//C     #endif /* __cplusplus */

//C     struct my_winhttp_stream {
//C         wchar_t     * szAppName;
//C         wchar_t     * szUrl;
//C         wchar_t     * szHostName;
//C         wchar_t     * szUrlPath;
//C         wchar_t     * szHeader;
    //char        * lpData;
    //unsigned long dwDataSize;
//C     #ifdef MY_WINHTTP_INTERNAL
//C         HANDLE         hHeap;
//C         URL_COMPONENTS urlComponents;
//C         HINTERNET      hSession, hConnect, hRequest;
//C     #endif
//C     };
struct my_winhttp_stream
{
    wchar *szAppName;
    wchar *szUrl;
    wchar *szHostName;
    wchar *szUrlPath;
    wchar *szHeader;
}


//C     extern struct my_winhttp_stream * my_winhttp_stream_open(const wchar_t *szAppName, const wchar_t *szUrl);
extern (C):
my_winhttp_stream * my_winhttp_stream_open(wchar *szAppName, wchar *szUrl);

//C     extern void my_winhttp_stream_close(struct my_winhttp_stream *stream);
void  my_winhttp_stream_close(my_winhttp_stream *stream);

//C     extern const char * my_winhttp_stream_read_all(struct my_winhttp_stream *stream, unsigned long *dwSizeOptional);
char * my_winhttp_stream_read_all(my_winhttp_stream *stream, uint *dwSizeOptional);

//C     #ifdef __cplusplus
//C     }
//C     #endif /* __cplusplus */
