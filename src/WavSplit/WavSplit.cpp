#include "WavSplit.h"

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
    FILE *log = stderr;
    FILE *pipe = stdin;

    int nResult = -1;
    const bool isInputPipe = (_tcslen(wavFilePath) == 1) && (wavFilePath[0] == '-');

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

        auto h = WavFileInfo::ReadFileHeader(fstream);
        h->Print(log);
        _ftprintf(log, _T("\n"));

        int nProgress = 0;
        __int64 countBytes = 0;

        countBytes += h->HeaderSize;

        auto outputFileNames = std::make_unique<std::unique_ptr<_TCHAR[]>[]>(h->NumChannels);
        for (int i = 0; i < h->NumChannels; ++i)
            outputFileNames[i] = std::make_unique<_TCHAR[]>(_MAX_PATH);

        auto channels = std::make_unique<WavChannel[]>(h->NumChannels);
        int countChannels = 0;

        _TCHAR inputDrive[_MAX_DRIVE];
        _TCHAR inputDir[_MAX_DIR];
        _TCHAR inputName[_MAX_FNAME];
        _TCHAR inputExt[_MAX_EXT];

        if (isInputPipe == false)
            _tsplitpath_s(wavFilePath, inputDrive, inputDir, inputName, inputExt);

        auto channelLongNames = std::make_unique<std::unique_ptr<_TCHAR[]>[]>(h->NumChannels);
        for (int i = 0; i < h->NumChannels; ++i)
            channelLongNames[i] = std::make_unique<_TCHAR[]>(_MAX_PATH);

        auto channelShortNames = std::make_unique<std::unique_ptr<_TCHAR[]>[]>(h->NumChannels);
        for (int i = 0; i < h->NumChannels; ++i)
            channelShortNames[i] = std::make_unique<_TCHAR[]>(_MAX_PATH);

        if (h->IsExtensible == false)
        {
            for (int c = 0; c < h->NumChannels; c++)
            {
                _stprintf_s(channelLongNames[c].get(), 64, _T("Channel%d\0"), c + 1);
                _stprintf_s(channelShortNames[c].get(), 32, _T("CH%d\0"), c + 1);
                channels[c].LongName = channelLongNames[c].get();
                channels[c].ShortName = channelShortNames[c].get();
                channels[c].Mask = WavChannelMask::NONE;
            }
        }
        else
        {
            int nWavMultiChannelTypes = (int)WavFileHeader::WavMultiChannelTypes.size();
            for (int c = 0; c < nWavMultiChannelTypes; c++)
            {
                auto &ch = WavFileHeader::WavMultiChannelTypes[c];
                uint32_t mask = static_cast<uint32_t>(ch.Mask);
                if ((mask & h->ChannelMask) != 0)
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
                _stprintf_s(outputFileNames[p].get(), _MAX_PATH, _T("%s\0"), outputFileNameNuffer);
                _ftprintf(log, _T("Output[%d]:\t%s\n"), p, outputFileNames[p].get());
            }
            else
            {
                _stprintf_s(fileNameBuffer, _MAX_FNAME, _T("%s.%s\0"), inputName, channels[p].ShortName);
                _tmakepath_s(outputFileNameNuffer, nullptr, outputPath, fileNameBuffer, inputExt);
                _stprintf_s(outputFileNames[p].get(), _MAX_PATH, _T("%s\0"), outputFileNameNuffer);
                _ftprintf(log, _T("Output[%d]:\t%s\n"), p, outputFileNames[p].get());
            }
        }

        uint32_t dataSize = h->Subchunk2Size;
        uint32_t bufferSize = h->ByteRate;
        uint32_t channelBufferSize = h->ByteRate / h->NumChannels;
        uint16_t copySize = h->BlockAlign / h->NumChannels;

        auto buffer = std::make_unique<unsigned char[]>(bufferSize);

        auto channelBuffer = std::make_unique<std::unique_ptr<unsigned char[]>[]>(h->NumChannels);
        for (int i = 0; i < h->NumChannels; ++i)
            channelBuffer[i] = std::make_unique<unsigned char[]>(channelBufferSize);

        auto outputFiles = std::make_unique<FILE*[]>(h->NumChannels);
        auto count = std::make_unique<int[]>(h->NumChannels);

        auto mh = WavFileInfo::GetMonoWavFileHeader(h.get());

        for (int c = 0; c < h->NumChannels; c++)
        {
            errno_t error = _tfopen_s(&outputFiles[c], outputFileNames[c].get(), _T("wb"));
            if (error != 0)
            {
                _ftprintf(log, _T("Failed to create output file."));
                nResult = -1;
                goto cleanup;
            }
        }

        for (int c = 0; c < h->NumChannels; c++)
        {
            WavFileInfo::WriteFileHeader(outputFiles[c], mh.get());
        }

        for (uint32_t i = 0; i < dataSize; i += bufferSize)
        {
            size_t n = fread(buffer.get(), sizeof(unsigned char), bufferSize, fstream);
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
                    fwrite(channelBuffer[c].get(), sizeof(unsigned char), count[c], outputFiles[c]);
                }

                countBytes += n;

                if (isInputPipe == false)
                {
                    nProgress = (int)(((double)countBytes / (double)nFileSize) * 100);
                    __int64 nCurPos = _ftelli64(fstream);
                    _ftprintf(log, _T("\rProgress: %d%% (%I64d/%I64d)"), nProgress, nCurPos, nFileSize);
                }
            }
        }

        nResult = 0;
		_ftprintf(log, _T("\nDone."));

    cleanup:

        for (int c = 0; c < h->NumChannels; c++)
            fclose(outputFiles[c]);

        if (isInputPipe == false)
            fclose(fstream);
    }
    catch (TCHAR *error)
    {
        _ftprintf(log, _T("\nFailed to split file: %s"), error);
        nResult = -1;
    }
    catch (...)
    {
        _ftprintf(log, _T("\nFailed to split file."));
        nResult = -1;
    }

    return nResult;
}
