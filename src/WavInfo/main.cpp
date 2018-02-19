#define WIN32_LEAN_AND_MEAN
#include <SDKDDKVer.h>
#include <windows.h>
#include <stdlib.h>  
#include <stdio.h>
#include <tchar.h>
#include "version.h"
#include "WavInfo.h"

void Info(FILE *log)
{
    _ftprintf(log, _T("%s v%s (%s %s %s, %s %s)\n"),
        _T(PRODUCTNAME),
        _T(VERSION),
#if defined(_WIN32) & !defined(_WIN64)
        _T("x86"),
#else
        _T("x64"),
#endif
#if defined(UNICODE)
        _T("Unicode"),
#else
        _T("Ansi"),
#endif
#if defined(_DEBUG)
        _T("Debug"),
#else
        _T("Release"),
#endif
        _T(__DATE__),
        _T(__TIME__));
    _ftprintf(log, _T("%s\n"), _T(COPYRIGHT));
}

void Help(FILE *log)
{
    _ftprintf(log, _T("usage: %s [option] <...>\n"), _T(PRODUCTNAME));
    _ftprintf(log, _T("option:\n"));
    _ftprintf(log, _T("\t[-i] Show wav file header info\n"));
    _ftprintf(log, _T("\texamples:\n"));
    _ftprintf(log, _T("\t%s -i <input.wav>\n"), _T(PRODUCTNAME));
}

int _tmain(int argc, _TCHAR* argv[])
{
    Info(stderr);

    if (argc != 3 && argc != 4)
    {
        Help(stderr);
        return -1;
    }

    const _TCHAR *option = argv[1];
    const size_t lenOption = _tcslen(option);
    if (lenOption != 2)
    {
        Help(stderr);
        return -1;
    }

    if (_tcsnccmp(option, _T("-i"), 2) == 0 && argc == 3)
    {
        const _TCHAR *wavFilePath = argv[2];

        return WavInfo(wavFilePath);
    }

    Help(stderr);
    return -1;
}
