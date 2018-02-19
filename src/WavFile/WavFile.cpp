#include "WavFile.h"

WavChannel::WavChannel()
{
}

WavChannel::WavChannel(_TCHAR * longName, _TCHAR * shortName, uint32_t mask)
{
    LongName = longName;
    ShortName = shortName;
    Mask = mask;
}

WavChannel::~WavChannel()
{
}

std::vector<WavChannel> WavFileHeader::WavChannelTypes = {
    { _T("Mono"),                    _T("M"),   WavChannelMask::NONE },
    { _T("Left"),                    _T("L"),   WavChannelMask::NONE },
    { _T("Right"),                   _T("R"),   WavChannelMask::NONE }
};

std::vector<WavChannel> WavFileHeader::WavMultiChannelTypes = {
    { _T("Front Left"),              _T("FL"),   WavChannelMask::SPEAKER_FRONT_LEFT               },
    { _T("Front Right"),             _T("FR"),   WavChannelMask::SPEAKER_FRONT_RIGHT              },
    { _T("Front Center"),            _T("FC"),   WavChannelMask::SPEAKER_FRONT_CENTER             },
    { _T("Low Frequency"),           _T("LF"),   WavChannelMask::SPEAKER_LOW_FREQUENCY            },
    { _T("Back Left"),               _T("BL"),   WavChannelMask::SPEAKER_BACK_LEFT                },
    { _T("Back Right"),              _T("BR"),   WavChannelMask::SPEAKER_BACK_RIGHT               },
    { _T("Front Left of Center"),    _T("FLC"),  WavChannelMask::SPEAKER_FRONT_LEFT_OF_CENTER     },
    { _T("Front Right of Center"),   _T("FRC"),  WavChannelMask::SPEAKER_FRONT_RIGHT_OF_CENTER    },
    { _T("Back Center"),             _T("BC"),   WavChannelMask::SPEAKER_BACK_CENTER              },
    { _T("Side Left"),               _T("SL"),   WavChannelMask::SPEAKER_SIDE_LEFT                },
    { _T("Side Right"),              _T("SR"),   WavChannelMask::SPEAKER_SIDE_RIGHT               },
    { _T("Top Center"),              _T("TC"),   WavChannelMask::SPEAKER_TOP_CENTER               },
    { _T("Top Front Left"),          _T("TFL"),  WavChannelMask::SPEAKER_TOP_FRONT_LEFT           },
    { _T("Top Front Center"),        _T("TFC"),  WavChannelMask::SPEAKER_TOP_FRONT_CENTER         },
    { _T("Top Front Right"),         _T("TFR"),  WavChannelMask::SPEAKER_TOP_FRONT_RIGHT          },
    { _T("Top Back Left"),           _T("TBL"),  WavChannelMask::SPEAKER_TOP_BACK_LEFT            },
    { _T("Top Back Center"),         _T("TBC"),  WavChannelMask::SPEAKER_TOP_BACK_CENTER          },
    { _T("Top Back Right"),          _T("TBR"),  WavChannelMask::SPEAKER_TOP_BACK_RIGHT           }
};

// 00000001-0000-0010-8000-00aa00389b71
GUID WavFileHeader::SubTypePCM = {
    0x00000001, 0x0000, 0x0010, { 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 }
};

// 00000003-0000-0010-8000-00aa00389b71
GUID WavFileHeader::SubTypeIEEE_FLOAT = {
    0x00000003, 0x0000, 0x0010, { 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 }
};

void WavFileHeader::Print(FILE *stream)
{
    // [WAVE]

    _ftprintf(stream, _T("[WAVE]\n"));

    if (ChunkID == 0x46464952)
        _ftprintf(stream, _T("ChunkID:\tRIFF\n"));
    else
        _ftprintf(stream, _T("ChunkID:\t%04x\n"), ChunkID);

    _ftprintf(stream, _T("ChunkSize:\t%d\n"), ChunkSize);

    if (Format == 0x45564157)
        _ftprintf(stream, _T("Format:\t\tWAVE\n"));
    else
        _ftprintf(stream, _T("Format:\t\t%04x\n"), Format);

    // [fmt]
    _ftprintf(stream, _T("[fmt]\n"));

    if (Subchunk1ID == 0x20746d66)
        _ftprintf(stream, _T("Subchunk1ID:\tfmt \n"));
    else
        _ftprintf(stream, _T("Subchunk1ID:\t%04x\n"), Subchunk1ID);

    _ftprintf(stream, _T("Subchunk1Size:\t%d\n"), Subchunk1Size);

    if (AudioFormat == 1)
        _ftprintf(stream, _T("AudioFormat:\t1 : PCM\n"));
    else if ((AudioFormat == (uint16_t)0xFFFE))
        _ftprintf(stream, _T("AudioFormat:\t0xFFFE : WAVEFORMATEXTENSIBLE\n"));
    else
        _ftprintf(stream, _T("AudioFormat:\t%02x\n"), AudioFormat);

    _ftprintf(stream, _T("NumChannels:\t%d\n"), NumChannels);
    _ftprintf(stream, _T("SampleRate:\t%d\n"), SampleRate);
    _ftprintf(stream, _T("ByteRate:\t%d\n"), ByteRate);
    _ftprintf(stream, _T("BlockAlign:\t%d\n"), BlockAlign);
    _ftprintf(stream, _T("BitsPerSample:\t%d\n"), BitsPerSample);

    // [extra]

    _ftprintf(stream, _T("[extra]\n"));
    _ftprintf(stream, _T("ExtraParamSize:\t%d\n"), ExtraParamSize);

    // [extensible]

    _ftprintf(stream, _T("[extensible]\n"));
    _ftprintf(stream, _T("Samples:\t%d\n"), Samples);
    _ftprintf(stream, _T("ChannelMask:\t%04x\n"), ChannelMask);

    const TCHAR *subTypeName;

    if (InlineIsEqualGUID(GuidSubFormat, SubTypePCM))
        subTypeName = _T("PCM");
    else if (InlineIsEqualGUID(GuidSubFormat, SubTypeIEEE_FLOAT))
        subTypeName = _T("IEEE FLOAT");
    else
        subTypeName = _T("Unknown");

    _ftprintf(stream,
        _T("GuidSubFormat:\t%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX : %s\n"),
        GuidSubFormat.Data1,
        GuidSubFormat.Data2,
        GuidSubFormat.Data3,
        GuidSubFormat.Data4[0], GuidSubFormat.Data4[1], GuidSubFormat.Data4[2], GuidSubFormat.Data4[3],
        GuidSubFormat.Data4[4], GuidSubFormat.Data4[5], GuidSubFormat.Data4[6], GuidSubFormat.Data4[7],
        subTypeName);

    // [data]

    _ftprintf(stream, _T("[data]\n"));

    if (Subchunk2ID == 0x61746164)
        _ftprintf(stream, _T("Subchunk2ID:\tdata\n"));
    else
        _ftprintf(stream, _T("Subchunk2ID:\t%04x\n"), Subchunk2ID);

    _ftprintf(stream, _T("Subchunk2Size:\t%d\n"), Subchunk2Size);

    // [info]

    _ftprintf(stream, _T("[info]\n"));
    _ftprintf(stream, _T("IsExtensible:\t%d\n"), IsExtensible);
    _ftprintf(stream, _T("HeaderSize:\t%d\n"), HeaderSize);
    _ftprintf(stream, _T("Duration:\t%f\n"), Duration);
    _ftprintf(stream, _T("TotalSamples:\t%d"), TotalSamples);
}

int WavFileInfo::fpeek(FILE *fs)
{
    int c;
    c = fgetc(fs);
    ungetc(c, fs);
    return c;
}

std::unique_ptr<WavFileHeader> WavFileInfo::ReadFileHeader(FILE *fs)
{
    auto h = std::make_unique<WavFileHeader>();

    h->HeaderSize = 0;

    // Read WAV header.

    // WAVE
    fread(&h->ChunkID, sizeof(uint32_t), 1, fs);    // 0x46464952, "RIFF"
    fread(&h->ChunkSize, sizeof(uint32_t), 1, fs);  // 36 + SubChunk2Size, 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
    fread(&h->Format, sizeof(uint32_t), 1, fs);     // 0x45564157, "WAVE"

    h->HeaderSize += 12;

    // fmt
    fread(&h->Subchunk1ID, sizeof(uint32_t), 1, fs);    // 0x20746d66, "fmt "
    fread(&h->Subchunk1Size, sizeof(uint32_t), 1, fs);  // 16 for PCM, 40 for WAVEFORMATEXTENSIBLE
    fread(&h->AudioFormat, sizeof(uint16_t), 1, fs);    // PCM = 1, WAVEFORMATEXTENSIBLE.SubFormat = 0xFFFE
    fread(&h->NumChannels, sizeof(uint16_t), 1, fs);    // Mono = 1, Stereo = 2, etc.
    fread(&h->SampleRate, sizeof(uint32_t), 1, fs);     // 8000, 44100, etc.
    fread(&h->ByteRate, sizeof(uint32_t), 1, fs);       // SampleRate * NumChannels * BitsPerSample/8
    fread(&h->BlockAlign, sizeof(uint16_t), 1, fs);     // NumChannels * BitsPerSample/8
    fread(&h->BitsPerSample, sizeof(uint16_t), 1, fs);  // 8 bits = 8, 16 bits = 16, etc.

    h->HeaderSize += 24;

    // Read PCM data or extensible data if exists.
    if (h->Subchunk1Size == 16 && h->AudioFormat == 1)
    {
        // PCM
        h->IsExtensible = false;

        // Note: 8-bit samples are stored as unsigned bytes, ranging from 0 to 255. 16-bit samples are stored as 2's-complement signed integers, ranging from -32768 to 32767.
        // data
        fread(&h->Subchunk2ID, sizeof(uint32_t), 1, fs);    // 0x61746164, "data"
        fread(&h->Subchunk2Size, sizeof(uint32_t), 1, fs);  // NumSamples * NumChannels * BitsPerSample/8

        h->HeaderSize += 8;
    }
    else if (h->Subchunk1Size > 16 && h->AudioFormat == 0xFFFE)
    {
        // read WAVEFORMATEXTENSIBLE
        fread(&h->ExtraParamSize, sizeof(uint16_t), 1, fs);
        h->HeaderSize += 2;

        if (h->ExtraParamSize == 22)
        {
            // if cbSize is set to 22 => WAVEFORMATEXTENSIBLE
            h->IsExtensible = true;

            //union {
            //    WORD wValidBitsPerSample; // bits of precision
            //    WORD wSamplesPerBlock;    // valid if wBitsPerSample==0
            //    WORD wReserved;           // If neither applies, set to zero.
            //} Samples;
            fread(&h->Samples, sizeof(uint16_t), 1, fs);

            // DWORD dwChannelMask; which channels are present in stream
            fread(&h->ChannelMask, sizeof(uint32_t), 1, fs);

            // GUID SubFormat
            fread(&h->GuidSubFormat, sizeof(GUID), 1, fs);

            h->HeaderSize += 22;

            // Check sub-format.
            if (!InlineIsEqualGUID(h->GuidSubFormat, WavFileHeader::SubTypePCM)
                && !InlineIsEqualGUID(h->GuidSubFormat, WavFileHeader::SubTypeIEEE_FLOAT))
            {
                throw _T("Not supported WAV file type"); // h->GuidSubFormat
            }

            // Find "data" chunk.
            while (fpeek(fs) != EOF)
            {
                uint32_t chunk;
                fread(&chunk, sizeof(uint32_t), 1, fs);
                h->HeaderSize += 4;

                if (chunk == 0x61746164)
                {
                    // "data" chunk
                    h->Subchunk2ID = chunk;                             // 0x61746164, "data"
                    fread(&h->Subchunk2Size, sizeof(uint32_t), 1, fs);  // NumSamples * NumChannels * BitsPerSample/8

                    h->HeaderSize += 4;

                    break;
                }
                else
                {
                    // Read other non "data" chunks.
                    uint32_t chunkSize;
                    fread(&chunkSize, sizeof(uint32_t), 1, fs);

                    h->HeaderSize += 4;

                    // char chunkName[4] = chunk;
                    auto chunkData = std::make_unique<unsigned char[]>(chunkSize);
                    fread(chunkData.get(), sizeof(unsigned char), chunkSize, fs);

                    h->HeaderSize += (int)chunkSize;
                }
            }
        }
        else
        {
            throw _T("Not supported WAV file header.");
        }
    }
    else
    {
        throw _T("Not supported WAV file header.");
    }

    // Calculate number of total samples.
    h->TotalSamples = (long)((double)h->Subchunk2Size / ((double)h->NumChannels * (double)h->BitsPerSample / 8));

    // Calculate duration in seconds.
    h->Duration = (1 / (double)h->SampleRate) * (double)h->TotalSamples;

    return h;
}

void WavFileInfo::WriteFileHeader(FILE *fs, const WavFileHeader *h)
{
    // Write WAV header.

    // WAVE
    fwrite(&h->ChunkID, sizeof(uint32_t), 1, fs); // 0x46464952, "RIFF"
    fwrite(&h->ChunkSize, sizeof(uint32_t), 1, fs);
    fwrite(&h->Format, sizeof(uint32_t), 1, fs); // 0x45564157, "WAVE"

    // fmt
    fwrite(&h->Subchunk1ID, sizeof(uint32_t), 1, fs); // 0x20746d66, "fmt "
    fwrite(&h->Subchunk1Size, sizeof(uint32_t), 1, fs);
    fwrite(&h->AudioFormat, sizeof(uint16_t), 1, fs);
    fwrite(&h->NumChannels, sizeof(uint16_t), 1, fs);
    fwrite(&h->SampleRate, sizeof(uint32_t), 1, fs);
    fwrite(&h->ByteRate, sizeof(uint32_t), 1, fs);
    fwrite(&h->BlockAlign, sizeof(uint16_t), 1, fs);
    fwrite(&h->BitsPerSample, sizeof(uint16_t), 1, fs);

    // Write PCM data or extensible data if exists.
    if (h->Subchunk1Size == 16 && h->AudioFormat == 1)
    {
        // PCM
        fwrite(&h->Subchunk2ID, sizeof(uint32_t), 1, fs); // 0x61746164, "data"
        fwrite(&h->Subchunk2Size, sizeof(uint32_t), 1, fs);
    }
    else if (h->Subchunk1Size > 16 && h->AudioFormat == 0xFFFE)
    {
        // Write WAVEFORMATEXTENSIBLE
        fwrite(&h->ExtraParamSize, sizeof(uint16_t), 1, fs);

        fwrite(&h->Samples, sizeof(uint16_t), 1, fs);
        fwrite(&h->ChannelMask, sizeof(uint32_t), 1, fs);
        fwrite(&h->GuidSubFormat, sizeof(GUID), 1, fs);

        fwrite(&h->Subchunk2ID, sizeof(uint32_t), 1, fs); // 0x61746164, "data"
        fwrite(&h->Subchunk2Size, sizeof(uint32_t), 1, fs);
    }
    else
    {
        throw _T("Not supported WAV file header.");
    }
}

std::unique_ptr<WavFileHeader> WavFileInfo::GetMonoWavFileHeader(const WavFileHeader *h)
{
    // Each mono output file has the same header.
    auto mh = std::make_unique<WavFileHeader>();

    // WAVE
    mh->ChunkID = (uint32_t)0x46464952; // 0x46464952, "RIFF"
    mh->ChunkSize = 36 + (h->Subchunk2Size / h->NumChannels); // 36 + SubChunk2Size, 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
    mh->Format = (uint32_t)0x45564157; // 0x45564157, "WAVE"

    // fmt
    mh->Subchunk1ID = (uint32_t)0x20746d66; // 0x20746d66, "fmt "
    mh->Subchunk1Size = 16; // 16 for PCM, 40 for WAVEFORMATEXTENSIBLE
    mh->AudioFormat = (uint16_t)1; // PCM = 1, WAVEFORMATEXTENSIBLE.SubFormat = 0xFFFE
    mh->NumChannels = (uint16_t)1; // Mono = 1, Stereo = 2, etc.
    mh->SampleRate = h->SampleRate; // 8000, 44100, etc.
    mh->ByteRate = (uint32_t)((h->SampleRate * 1 * h->BitsPerSample) / 8); // SampleRate * NumChannels * BitsPerSample/8
    mh->BlockAlign = (uint16_t)((1 * h->BitsPerSample) / 8); // NumChannels * BitsPerSample/8
    mh->BitsPerSample = h->BitsPerSample; // 8 bits = 8, 16 bits = 16, etc.

    // extensible
    mh->ExtraParamSize = (uint16_t)0;
    mh->ChannelMask = (uint32_t)0;
    mh->GuidSubFormat = { 0x00000000, 0x0000, 0x0000,{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };

    // data
    mh->Subchunk2ID = (uint32_t)0x61746164; // 0x61746164, "data"
    mh->Subchunk2Size = h->Subchunk2Size / h->NumChannels; // NumSamples * NumChannels * BitsPerSample/8

    // info
    mh->IsExtensible = false;
    mh->HeaderSize = 44;
    mh->TotalSamples = (long)((double)mh->Subchunk2Size / ((double)mh->NumChannels * (double)mh->BitsPerSample / 8));
    mh->Duration = (1 / (double)mh->SampleRate) * (double)mh->TotalSamples;

    return mh;
}
