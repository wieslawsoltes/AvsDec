#define WIN32_LEAN_AND_MEAN
#include "targetver.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "Avs2Raw.h"

int AvsDec(char *avsFilePath, char *rawFilePath)
{
    FILE *log = stderr;
    FILE *pipe = stdout;

    bool isOutputPipe = (strlen(rawFilePath) == 1) && (rawFilePath[0] == '-');

    CAvs2Raw decoderAVS;
    if (decoderAVS.OpenAvisynth(avsFilePath) == false)
    {
        _ftprintf(log, _T("Failed to initialize Avisynth!"));
        return -1;
    }

    _ftprintf(log, _T("Avisynth initialized successfully.\n"));

    AvsAudioInfo infoAVS;
    memset(&infoAVS, 0, sizeof(AvsAudioInfo));
    infoAVS = decoderAVS.GetInputInfo();

    ULONGLONG  nFileSize = infoAVS.nAudioSamples * infoAVS.nBytesPerChannelSample * infoAVS.nAudioChannels;

    _ftprintf(log, _T("AVS File Properties\n"));
    _ftprintf(log, _T("Sample format\t: "));
    switch (infoAVS.nSampleType)
    {
    case SAMPLE_INT8: _ftprintf(log, _T("SAMPLE_INT8\n")); break;
    case SAMPLE_INT16: _ftprintf(log, _T("SAMPLE_INT16\n")); break;
    case SAMPLE_INT24: _ftprintf(log, _T("SAMPLE_INT24\n")); break;
    case SAMPLE_INT32: _ftprintf(log, _T("SAMPLE_INT32\n")); break;
    case SAMPLE_FLOAT: _ftprintf(log, _T("SAMPLE_FLOAT\n")); break;
    default: _ftprintf(log, _T("unknown\n")); break;
    }
    _ftprintf(log, _T("%s\t: %d\n"), _T("Sample rate"), infoAVS.nSamplesPerSecond);
    _ftprintf(log, _T("%s\t: %d\n"), _T("Channels"), infoAVS.nAudioChannels);
    _ftprintf(log, _T("%s\t: %I64d\n"), _T("Audio samples"), infoAVS.nAudioSamples);
    _ftprintf(log, _T("%s\t: %I64d\n"), _T("Decoded size"), nFileSize);

    int nSamples = 1536;
    Avs2RawStatus statusAVS;
    statusAVS.nStart = 0;
    statusAVS.nSamples = infoAVS.nAudioSamples;
    statusAVS.nSamplesLeft = infoAVS.nAudioSamples;
    statusAVS.nSamplesToRead = nSamples;

    FLOAT *pBuffer;
    pBuffer = (FLOAT *)calloc(nSamples * infoAVS.nAudioChannels, sizeof(FLOAT));
    if (pBuffer == nullptr)
    {
        _ftprintf(log, _T("Failed to allocate memory."));
        decoderAVS.CloseAvisynth();
        return -1;
    }

    FILE *fstream;

    if (isOutputPipe == true)
    {
        fstream = pipe;
        _setmode(_fileno(pipe), O_BINARY);
    }
    else
    {
        errno_t error = fopen_s(&fstream, rawFilePath, "wb");
        if (error != 0)
        {
            _ftprintf(log, _T("Failed to create output file."));
            decoderAVS.CloseAvisynth();
            return -1;
        }
    }

    int nr;
    ULONGLONG nDecodedSamples = 0;
    do
    {
        statusAVS.nSamplesToRead = nSamples;
        nr = decoderAVS.GetAudio(pBuffer, &statusAVS);
        nDecodedSamples += nr;
        fwrite(pBuffer, sizeof(FLOAT), nr * infoAVS.nAudioChannels, fstream);

        int nProgress = (int)((100 * nDecodedSamples * infoAVS.nBytesPerChannelSample * infoAVS.nAudioChannels) / nFileSize);
        _ftprintf(log, _T("\rProgress: %d%% (%I64d/%I64d)"), nProgress, statusAVS.nSamples - statusAVS.nSamplesLeft, statusAVS.nSamples);
    } while (nr > 0);

    if (isOutputPipe == false)
        fclose(fstream);

    decoderAVS.CloseAvisynth();

    return 0;
}
