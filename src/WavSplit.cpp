#define WIN32_LEAN_AND_MEAN
#include "targetver.h"
#include <windows.h>
#include <stdlib.h> 
#include <stdio.h>
#include <tchar.h>
#include <fcntl.h>
#include <io.h>
#include "WavFile.h"

__int64 GetFileSize(FILE *fp)
{
    __int64 nCurPos, nSize;
    nCurPos = _ftelli64(fp);
    _fseeki64(fp, 0, SEEK_END);
    nSize = _ftelli64(fp);
    _fseeki64(fp, nCurPos, SEEK_SET);
    return nSize;
}

int WavSplit(const _TCHAR *wavFilePath, const _TCHAR *outputPath)
{
    int nResult = -1;
    FILE *log = stdout;
    FILE *pipe = stdin;

    bool isInputPipe = (_tcslen(wavFilePath) == 1) && (wavFilePath[0] == '-');

    try
    {
        FILE *fstream;

        if (isInputPipe == true)
        {
            fstream = pipe;
            _setmode(_fileno(pipe), O_BINARY);
        }
        else
        {
            errno_t error = _tfopen_s(&fstream, wavFilePath, _T("rb"));
            if (error != 0)
            {
                _ftprintf(log, _T("Failed to open wav file."));
                return -1;
            }
        }

        __int64 nFileSize = 0;

        if (isInputPipe == false)
        {
            _ftprintf(log, _T("File:\t\t%s\n"), wavFilePath);
            nFileSize = GetFileSize(fstream);
            _ftprintf(log, _T("File size:\t%I64d\n"), nFileSize);
        }
 
        WavFileHeader *h = WavFileInfo::ReadFileHeader(fstream);
        h->Print(log);
        _ftprintf(log, _T("\n"));

        int nProgress = 0;
        __int64 countBytes = 0;

        countBytes += h->HeaderSize;

        _TCHAR** outputFileNames;
        outputFileNames = new _TCHAR*[h->NumChannels];
        for (int i = 0; i < h->NumChannels; ++i)
            outputFileNames[i] = new _TCHAR[_MAX_PATH];

        WavChannel *channels = new WavChannel[h->NumChannels];
        int countChannels = 0;

        _TCHAR inputDrive[_MAX_DRIVE];
        _TCHAR inputDir[_MAX_DIR];
        _TCHAR inputName[_MAX_FNAME];
        _TCHAR inputExt[_MAX_EXT];

        if (isInputPipe == false)
            _tsplitpath_s(wavFilePath, inputDrive, inputDir, inputName, inputExt);

        _TCHAR** channelLongNames;
        channelLongNames = new _TCHAR*[h->NumChannels];
        for (int i = 0; i < h->NumChannels; ++i)
            channelLongNames[i] = new _TCHAR[64];

        _TCHAR** channelShortNames;
        channelShortNames = new _TCHAR*[h->NumChannels];
        for (int i = 0; i < h->NumChannels; ++i)
            channelShortNames[i] = new _TCHAR[32];

        if (h->IsExtensible == false)
        {
            for (int c = 0; c < h->NumChannels; c++)
            {
                _stprintf_s(channelLongNames[c], 64, _T("Channel%d\0"), c + 1);
                _stprintf_s(channelShortNames[c], 32, _T("CH%d\0"), c + 1);
                WavChannel ch(channelLongNames[c], channelShortNames[c], WavChannelMask::NONE);
                channels[c] = ch;
            }
        }
        else
        {
            for (int c = 0; c < WavFileHeader::nWavMultiChannelTypes; c++)
            {
                WavChannel &ch = WavFileHeader::WavMultiChannelTypes[c];
                if ((ch.Mask & h->ChannelMask) != 0)
                {
                    channels[countChannels++] = ch;
                }
            }
        }

        _TCHAR fileNameBuffer[_MAX_FNAME];
        _TCHAR outputFileNameNuffer[_MAX_PATH];

        for (int p = 0; p < h->NumChannels; p++)
        {
            if (isInputPipe == true)
            {
                _stprintf_s(fileNameBuffer, _MAX_FNAME, _T("%s\0"), channels[p].ShortName);
                _tmakepath_s(outputFileNameNuffer, nullptr, outputPath, fileNameBuffer, _T("wav"));
                _stprintf_s(outputFileNames[p], _MAX_PATH, _T("%s\0"), outputFileNameNuffer);
                _ftprintf(log, _T("Output[%d]:\t%s\n"), p, outputFileNames[p]);
            }
            else
            {
                _stprintf_s(fileNameBuffer, _MAX_FNAME, _T("%s.%s\0"), inputName, channels[p].ShortName);
                _tmakepath_s(outputFileNameNuffer, nullptr, outputPath, fileNameBuffer, inputExt);
                _stprintf_s(outputFileNames[p], _MAX_PATH, _T("%s\0"), outputFileNameNuffer);
                _ftprintf(log, _T("Output[%d]:\t%s\n"), p, outputFileNames[p]);
            }
        }

        uint32_t dataSize = h->Subchunk2Size;
        uint32_t bufferSize = h->ByteRate;
        uint32_t channelBufferSize = h->ByteRate / h->NumChannels;
        uint16_t copySize = h->BlockAlign / h->NumChannels;

        unsigned char* buffer = new unsigned char[bufferSize];
        unsigned char** channelBuffer = new unsigned char*[h->NumChannels];
        FILE** outputFiles = new FILE*[h->NumChannels];
        WavFileHeader* mh = WavFileInfo::GetMonoWavFileHeader(h);

        for (int c = 0; c < h->NumChannels; c++)
        {
            channelBuffer[c] = new unsigned char[channelBufferSize];
        }

        for (int c = 0; c < h->NumChannels; c++)
        {
            errno_t error = _tfopen_s(&outputFiles[c], outputFileNames[c], _T("wb"));
            if (error != 0)
            {
                _ftprintf(log, _T("Failed to create output file."));
                nResult = -1;
                goto cleanup;
            }
        }

        for (int c = 0; c < h->NumChannels; c++)
        {
            WavFileInfo::WriteFileHeader(outputFiles[c], mh);
        }

        int *count = new int[h->NumChannels];

        for (uint32_t i = 0; i < dataSize; i += bufferSize)
        {
            size_t n = fread(buffer, sizeof(unsigned char), bufferSize, fstream);
            if (n > 0)
            {
                for (uint16_t c = 0; c < h->NumChannels; c++)
                    count[c] = 0;

                for (size_t j = 0; j < n; j += h->BlockAlign)
                {
                    for (uint16_t c = 0; c < h->NumChannels; c++)
                    {
                        for (uint16_t k = 0; k < copySize; k++)
                        {
                            channelBuffer[c][count[c]++] = buffer[j + (c * copySize) + k];
                        }
                    }
                }

                for (uint16_t c = 0; c < h->NumChannels; c++)
                {
                    fwrite(channelBuffer[c], sizeof(unsigned char), count[c], outputFiles[c]);
                }

                countBytes += n;

                if (isInputPipe == false)
                {
                    nProgress = (int)(((countBytes / nFileSize) * 100.0));
                    __int64 nCurPos = _ftelli64(fstream);
                    _ftprintf(log, _T("\rProgress: %d%% (%I64d/%I64d)"), nProgress, nCurPos, nFileSize);
                }
            }
        }

        nResult = 0;

        delete[] count;

cleanup:
        for (int i = 0; i < h->NumChannels; ++i)
            delete[] outputFileNames[i];
        delete[] outputFileNames;

        delete[] channels;

        for (int c = 0; c < h->NumChannels; c++)
        {
            fflush(outputFiles[c]);
            fclose(outputFiles[c]);
        }

        for (int i = 0; i < h->NumChannels; ++i)
            delete[] channelLongNames[i];
        delete[] channelLongNames;

        for (int i = 0; i < h->NumChannels; ++i)
            delete[] channelShortNames[i];
        delete[] channelShortNames;

        delete[] buffer;

        for (int i = 0; i < h->NumChannels; ++i)
            delete[] channelBuffer[i];
        delete[] channelBuffer;

        delete[] outputFiles;

        delete mh;

        if (isInputPipe == false)
            fclose(fstream);

        delete h;
    }
    catch (TCHAR *error)
    {
        _ftprintf(log, _T("Failed to split file: %s"), error);
        nResult = -1;
    }
    catch (...)
    {
        _ftprintf(log, _T("Failed to split file."));
        nResult = -1;
    }

    return nResult;
}
