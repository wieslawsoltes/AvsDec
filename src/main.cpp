#define WIN32_LEAN_AND_MEAN
#include <SDKDDKVer.h>
#include <windows.h>
#include <stdlib.h>  
#include <stdio.h>
#include <tchar.h>
#include "version.h"
#include "AvsDec.h"
#include "WavInfo.h"
#include "WavSplit.h"

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
    _ftprintf(log, _T("\t[-d] Decode avisynth audio stream to raw audio file\n"));
    _ftprintf(log, _T("\texamples:\n"));
    _ftprintf(log, _T("\t%s -d <input.avs> <output.raw>\n"), _T(PRODUCTNAME));
    _ftprintf(log, _T("\t%s -d <input.avs> - > <output.raw>\n"), _T(PRODUCTNAME));
    _ftprintf(log, _T("\n"));
    _ftprintf(log, _T("\t[-i] Show wav file header info\n"));
    _ftprintf(log, _T("\texamples:\n"));
    _ftprintf(log, _T("\t%s -i <input.wav>\n"), _T(PRODUCTNAME));
    _ftprintf(log, _T("\n"));
    _ftprintf(log, _T("\t[-s] Split multi-channel WAV file into single channel WAV files\n"));
    _ftprintf(log, _T("\texamples:\n"));
    _ftprintf(log, _T("\t%s -s <input.wav> [<OutputPath>]\n"), _T(PRODUCTNAME));
    _ftprintf(log, _T("\t%s -s - [<OutputPath>] < <input.wav>\n"), _T(PRODUCTNAME));
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

    if (_tcsnccmp(option, _T("-d"), 2) == 0 && argc == 4)
    {
        const _TCHAR *avsFilePath = argv[2];
        const _TCHAR *rawFilePath = argv[3];
        return AvsDec(avsFilePath, rawFilePath);
    }

    if (_tcsnccmp(option, _T("-i"), 2) == 0 && argc == 3)
    {
        const _TCHAR *wavFilePath = argv[2];
        return WavInfo(wavFilePath);
    }

    if (_tcsnccmp(option, _T("-s"), 2) == 0 && (argc == 3 || argc == 4))
    {
        if (argc == 3)
        {
            const _TCHAR *wavFilePath = argv[2];
            const bool isInputPipe = (_tcslen(wavFilePath) == 1) && (wavFilePath[0] == '-');

            if (isInputPipe == false)
            {
                _TCHAR drive[_MAX_DRIVE];
                _TCHAR dir[_MAX_DIR];
                _TCHAR fname[_MAX_FNAME];
                _TCHAR ext[_MAX_EXT];
                _TCHAR outputPath[_MAX_PATH];
                _TCHAR fullwavFilePath[_MAX_PATH];
                _tfullpath(fullwavFilePath, wavFilePath, _MAX_PATH);
                _tsplitpath_s(fullwavFilePath, drive, dir, fname, ext);
                _tmakepath_s(outputPath, drive, dir, nullptr, nullptr);

                return WavSplit(fullwavFilePath, outputPath);
            }
            else
            {
                return WavSplit(wavFilePath, _T(""));
            }
        }
        else
        {
            const _TCHAR *wavFilePath = argv[2];
            const bool isInputPipe = (_tcslen(wavFilePath) == 1) && (wavFilePath[0] == '-');

            if (isInputPipe == false)
            {
                const _TCHAR *outputPath = argv[3];
                _TCHAR fullwavFilePath[_MAX_PATH];
                _TCHAR fullOutputPath[_MAX_PATH];
                _tfullpath(fullwavFilePath, wavFilePath, _MAX_PATH);
                _tfullpath(fullOutputPath, outputPath, _MAX_PATH);

                return WavSplit(fullwavFilePath, fullOutputPath);
            }
            else
            {
                const _TCHAR *outputPath = argv[3];
                _TCHAR fullOutputPath[_MAX_PATH];
                _tfullpath(fullOutputPath, outputPath, _MAX_PATH);

                return WavSplit(wavFilePath, fullOutputPath);
            }
        }
    }

    Help(stderr);
    return -1;
}
