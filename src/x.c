int printf (const char *__format, ...);
#include <windows.h>
#include <winhttp.h>

#define MY_WINHTTP_INTERNAL 1
#include "x.h"


extern struct my_winhttp_stream * my_winhttp_stream_open(const wchar_t *szAppName, const wchar_t *szUrl) {
    //HANDLE hHeap0 = HeapCreate(0, 0, 0);
    HANDLE hHeap0 = HeapCreate(0, 1024*1024*128, 0);
    printf("hHeap=0x%08x\n", hHeap0);

    //void* dummy = (void *)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, 1024 * 1024 * 512);
    //printf("dummy=0x%08x\n", dummy);

    struct my_winhttp_stream * stream = (struct my_winhttp_stream *)HeapAlloc(hHeap0, HEAP_ZERO_MEMORY, sizeof(struct my_winhttp_stream));
    stream->hHeap = hHeap0;

    stream->szAppName = (wchar_t *)HeapAlloc(stream->hHeap, HEAP_ZERO_MEMORY, sizeof(wchar_t) * (lstrlenW(szAppName)+1));
    lstrcpyW(stream->szAppName, szAppName);

    stream->szUrl = (wchar_t *)HeapAlloc(stream->hHeap, HEAP_ZERO_MEMORY, sizeof(wchar_t) * (lstrlenW(szUrl)+1));
    lstrcpyW(stream->szUrl, szUrl);

    printf("stream->szAppName=%ls\n", stream->szAppName);
    printf("stream->szUrl=%ls\n", stream->szUrl);

    stream->szHostName = (wchar_t *)HeapAlloc(stream->hHeap, HEAP_ZERO_MEMORY, sizeof(wchar_t) * (lstrlenW(szUrl)+1));
    stream->szUrlPath  = (wchar_t *)HeapAlloc(stream->hHeap, HEAP_ZERO_MEMORY, sizeof(wchar_t) * (lstrlenW(szUrl)+1));

    //URL_COMPONENTS urlComponents;
    ZeroMemory( &stream->urlComponents, sizeof(URL_COMPONENTS) );
    stream->urlComponents.dwStructSize     = sizeof(URL_COMPONENTS);
    stream->urlComponents.lpszHostName     = stream->szHostName;
    stream->urlComponents.dwHostNameLength = lstrlenW(szUrl)+1;
    stream->urlComponents.lpszUrlPath      = stream->szUrlPath;
    stream->urlComponents.dwUrlPathLength  = lstrlenW(szUrl)+1;

    if( !WinHttpCrackUrl( szUrl,
                          lstrlenW(szUrl),
                          0,
                          &stream->urlComponents ) )
    {
        // URLの解析に失敗
        printf("WinHttpCrackUrl() fail.\n");
        return 0;
    }
    printf("stream->szHostName=%ls\n", stream->szHostName);
    printf("stream->szUrlPath=%ls\n", stream->szUrlPath);
    printf("urlComponents.nPort=%u\n", stream->urlComponents.nPort);

    stream->hSession = WinHttpOpen(stream->szAppName, WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (stream->hSession == NULL) {
        HeapDestroy(stream->hHeap);
        return 0;
    }

    printf("(1)\n");

    stream->hConnect = WinHttpConnect(stream->hSession, stream->szHostName, stream->urlComponents.nPort, 0);
    if (stream->hConnect == NULL) {
        WinHttpCloseHandle(stream->hSession);
        HeapDestroy(stream->hHeap);
        return 0;
    }

    printf("(2)\n");

    // HTTPかHTTPSかそれ以外か
    DWORD dwOpenRequestFlag = (INTERNET_SCHEME_HTTPS == stream->urlComponents.nScheme) ? WINHTTP_FLAG_SECURE : 0;
    printf("dwOpenRequestFlag=0x%08lx\n", dwOpenRequestFlag);
    stream->hRequest = WinHttpOpenRequest(stream->hConnect, L"GET", stream->szUrlPath, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, dwOpenRequestFlag );
    if (stream->hRequest == NULL) {
        WinHttpCloseHandle(stream->hConnect);
        WinHttpCloseHandle(stream->hSession);
        HeapDestroy(stream->hHeap);
        return 0;
    }

    printf("(3)\n");

    if (!WinHttpSendRequest(stream->hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, WINHTTP_IGNORE_REQUEST_TOTAL_LENGTH, 0)) {
        WinHttpCloseHandle(stream->hRequest);
        WinHttpCloseHandle(stream->hConnect);
        WinHttpCloseHandle(stream->hSession);
        HeapDestroy(stream->hHeap);
        return 0;
    }

    printf("(4)\n");

    WinHttpReceiveResponse(stream->hRequest, NULL);

    DWORD dwSizeWithZero = 0; //unit = sizeof(wchar_t)
    WinHttpQueryHeaders(stream->hRequest, WINHTTP_QUERY_RAW_HEADERS_CRLF, WINHTTP_HEADER_NAME_BY_INDEX, NULL, &dwSizeWithZero, WINHTTP_NO_HEADER_INDEX);
    stream->szHeader = (wchar_t *)HeapAlloc(stream->hHeap, 0, dwSizeWithZero);
    WinHttpQueryHeaders(stream->hRequest, WINHTTP_QUERY_RAW_HEADERS_CRLF, WINHTTP_HEADER_NAME_BY_INDEX, stream->szHeader, &dwSizeWithZero, WINHTTP_NO_HEADER_INDEX);

    printf("(5)\n%ls\n", stream->szHeader);

    return stream;
}

extern void my_winhttp_stream_close(struct my_winhttp_stream *stream) {
    if (!stream) return;
    WinHttpCloseHandle(stream->hRequest);
    WinHttpCloseHandle(stream->hConnect);
    WinHttpCloseHandle(stream->hSession);
    HeapDestroy(stream->hHeap);
}

extern const char * my_winhttp_stream_read_all(struct my_winhttp_stream *stream, unsigned long *dwSizeOptional) {
    if (!stream) return NULL;

    char * lpData = NULL;
    DWORD  dwSize = 0;
    DWORD  dwTotalSize = 0;
    HANDLE hHeap = stream->hHeap;

    for (;;) {
        WinHttpQueryDataAvailable(stream->hRequest, &dwSize);
        if (!dwSize) break;
        DWORD dwSizeWithZero = dwTotalSize+dwSize+1;
        if (!lpData) {
            lpData = (char *)HeapAlloc(hHeap, 0, dwSizeWithZero);
        } else {
            lpData = (char *)HeapReAlloc(hHeap, 0, lpData, dwSizeWithZero);
        }
        WinHttpReadData(stream->hRequest, lpData + dwTotalSize, dwSize, NULL);
        dwTotalSize += dwSize;
        lpData[dwTotalSize] = 0;
    }

    if (dwSizeOptional) *dwSizeOptional= dwTotalSize;

    return lpData;
}

#ifdef TEST_MAIN
int main() {
    printf("start!\n");

    wchar_t * wszUrl = L"https://raw.githubusercontent.com/cyginst/ms2inst-v1/master/ms2inst.bat";
    wchar_t * wszAppName = L"Sample Application/1.0";

    struct my_winhttp_stream *stream = my_winhttp_stream_open(wszAppName, wszUrl);

    if (!stream) return 0;

    const char *lpData = my_winhttp_stream_read_all(stream, NULL);
    MessageBoxA(NULL, (LPSTR)lpData, "ボディ", MB_OK);

    my_winhttp_stream_close(stream);

    return 0;
}
#endif
