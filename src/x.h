#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct my_winhttp_stream {
    wchar_t     * szAppName;
    wchar_t     * szUrl;
    wchar_t     * szHostName;
    wchar_t     * szUrlPath;
    wchar_t     * szHeader;
    //char        * lpData;
    //unsigned long dwDataSize;
#ifdef MY_WINHTTP_INTERNAL
    HANDLE         hHeap;
    URL_COMPONENTS urlComponents;
    HINTERNET      hSession, hConnect, hRequest;
#endif
};


extern struct my_winhttp_stream * my_winhttp_stream_open(const wchar_t *szAppName, const wchar_t *szUrl);

extern void my_winhttp_stream_close(struct my_winhttp_stream *stream);

extern const char * my_winhttp_stream_read_all(struct my_winhttp_stream *stream, unsigned long *dwSizeOptional);

#ifdef __cplusplus
}
#endif /* __cplusplus */
