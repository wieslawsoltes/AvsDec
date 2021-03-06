﻿#include "WavInfo.h"

int WavInfo(const _TCHAR *wavFilePath)
{
    FILE *log = stderr;
    FILE *output = stdout;

    try
    {
        FILE *fstream;
        errno_t error = _tfopen_s(&fstream, wavFilePath, _T("rb"));
        if (error != 0)
        {
            _ftprintf(log, _T("Failed to open wav file."));
            return -1;
        }

        _ftprintf(output, _T("File:\t\t%s\n"), wavFilePath);

        auto h = WavFileInfo::ReadFileHeader(fstream);
        h->Print(output);

        fclose(fstream);
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

	_ftprintf(log, _T("\nDone."));
    return 0;
}
