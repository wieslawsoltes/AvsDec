#define VERSION _T("1.0")
#define WIN32_LEAN_AND_MEAN
#include "targetver.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "WavInfo.h"
#include "AvsDec.h"

void Help(FILE *log)
{
    _ftprintf(log, _T("usage: AvsDec [option] <...>\n"));
    _ftprintf(log, _T("option:"));
    _ftprintf(log, _T("\t[-i] Show wav file header info\n"));
    _ftprintf(log, _T("\tAvsDec -i <input.wav>\n"));
    _ftprintf(log, _T("\t[-d] Decode avisynth audio stream to raw audio file\n"));
    _ftprintf(log, _T("\tAvsDec -d <input.avs> <output.raw>\n"));
}

int main(int argc, char *argv[])
{
    _ftprintf(stderr, _T("AvsDec v%s\n"), VERSION);

    if (argc != 3 && argc != 4)
    {
        Help(stderr);
        return -1;
    }

    char *option = argv[1];
    size_t lenOption = strlen(option);
    if (lenOption != 2)
    {
        Help(stderr);
        return -1;
    }

    if (strncmp(option, "-i", 2) == 0 && argc == 3)
    {
        char *wavFilePath = argv[2];
        return WavInfo(wavFilePath);
    }

    if (strncmp(option, "-d", 2) == 0 && argc == 4)
    {
        char *avsFilePath = argv[2];
        char *rawFilePath = argv[3];
        return AvsDec(avsFilePath, rawFilePath);
    }

    Help(stderr);
    return -1;
}
