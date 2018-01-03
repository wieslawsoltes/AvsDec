#define WIN32_LEAN_AND_MEAN
#include "targetver.h"
#include <windows.h>
#include <stdlib.h> 
#include <stdio.h>
#include <tchar.h>
#include "WavFile.h"

int WavInfo(const _TCHAR *wavFilePath)
{
    FILE *log = stdout;

    try
    {
        FILE *fstream;
        errno_t error = _tfopen_s(&fstream, wavFilePath, _T("rb"));
        if (error != 0)
        {
            _ftprintf(log, _T("Failed to open wav file."));
            return -1;
        }

        _ftprintf(log, _T("File:\t\t%s\n"), wavFilePath);

        WavFileHeader *h = WavFileInfo::ReadFileHeader(fstream);
        h->Print(log);

        fclose(fstream);
        delete h;
    }
    catch (TCHAR *error)
    {
        _ftprintf(log, _T("Failed to read file header: %s"), error);
        return -1;
    }
    catch (...)
    {
        _ftprintf(log, _T("Failed to read file header."));
        return -1;
    }

    return 0;
}
