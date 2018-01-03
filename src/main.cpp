#define WIN32_LEAN_AND_MEAN
#include "version.h"
#include "targetver.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "AvsDec.h"
#include "WavInfo.h"

void Help(FILE *log)
{
    _ftprintf(log, _T("usage: AvsDec [option] <...>\n"));
    _ftprintf(log, _T("option:"));
    _ftprintf(log, _T("\t[-d] Decode avisynth audio stream to raw audio file\n"));
    _ftprintf(log, _T("\tAvsDec -d <input.avs> <output.raw>\n"));
    _ftprintf(log, _T("\t[-i] Show wav file header info\n"));
    _ftprintf(log, _T("\tAvsDec -i <input.wav>\n"));
}

int _tmain(int argc, _TCHAR* argv[])
{
    _ftprintf(stderr, _T("AvsDec v%s\n"), VERSION);

    if (argc != 3 && argc != 4)
    {
        Help(stderr);
        return -1;
    }

    _TCHAR *option = argv[1];
    size_t lenOption = _tcslen(option);
    if (lenOption != 2)
    {
        Help(stderr);
        return -1;
    }

    if (_tcsnccmp(option, _T("-d"), 2) == 0 && argc == 4)
    {
        _TCHAR *avsFilePath = argv[2];
        _TCHAR *rawFilePath = argv[3];
        return AvsDec(avsFilePath, rawFilePath);
    }

    if (_tcsnccmp(option, _T("-i"), 2) == 0 && argc == 3)
    {
        _TCHAR *wavFilePath = argv[2];
        return WavInfo(wavFilePath);
    }

    Help(stderr);
    return -1;
}
