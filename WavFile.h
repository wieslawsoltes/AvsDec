#pragma once

#include <cstdint>
#include <stdio.h>
#include <tchar.h>
#include <Guiddef.h>

// Multi-channel WAV file mask.
enum WavChannelMask
{
    NONE = 0x0,

    // Speaker front left channel mask.
    SPEAKER_FRONT_LEFT = 0x1,

    // Speaker front right channel mask.
    SPEAKER_FRONT_RIGHT = 0x2,

    // Speaker front center channel mask.
    SPEAKER_FRONT_CENTER = 0x4,

    // Speaker low frequency channel mask.
    SPEAKER_LOW_FREQUENCY = 0x8,

    // Speaker back left channel mask.
    SPEAKER_BACK_LEFT = 0x10,

    // Speaker back right channel mask.
    SPEAKER_BACK_RIGHT = 0x20,

    // Speaker front left of center channel mask.
    SPEAKER_FRONT_LEFT_OF_CENTER = 0x40,

    // Speaker front right of center channel mask.
    SPEAKER_FRONT_RIGHT_OF_CENTER = 0x80,

    // Speaker back center channel mask.
    SPEAKER_BACK_CENTER = 0x100,

    // Speaker side left channel mask.
    SPEAKER_SIDE_LEFT = 0x200,

    // Speaker side right channel mask.
    SPEAKER_SIDE_RIGHT = 0x400,

    // Speaker top center channel mask.
    SPEAKER_TOP_CENTER = 0x800,

    // Speaker top front left channel mask.
    SPEAKER_TOP_FRONT_LEFT = 0x1000,

    // Speaker top front center channel mask.
    SPEAKER_TOP_FRONT_CENTER = 0x2000,

    // Speaker top front right channel mask.
    SPEAKER_TOP_FRONT_RIGHT = 0x4000,

    // Speaker top back left channel mask.
    SPEAKER_TOP_BACK_LEFT = 0x8000,

    // Speaker top back center channel mask.
    SPEAKER_TOP_BACK_CENTER = 0x10000,

    // Speaker top back right channel mask.
    SPEAKER_TOP_BACK_RIGHT = 0x20000
};

/// WAV channel information.
class WavChannel
{
public:
    /// Wav channel long name.
    const char * LongName;
    // Wav channel short name.
    const char * ShortName;
    // Wav channel mask.
    WavChannelMask Mask;
public:
    WavChannel()
    {
    }
    WavChannel(const char * longName, const char * shortName, WavChannelMask mask)
    {
        LongName = longName;
        ShortName = shortName;
        Mask = mask;
    }
    virtual ~WavChannel()
    {
    }
};

/// The canonical WAVE format starts with the RIFF header.
class WavFileHeader
{
public:
    // WAVE                             bytes=12

    // Chunk ID.
    uint32_t ChunkID;          //  bytes=4

    // Chunk size.
    uint32_t ChunkSize;        //  bytes=4

    // Format.
    uint32_t Format;           //  bytes=4

    // fmt                              bytes=24

    // Sub-chunk 1 ID.
    uint32_t Subchunk1ID;      //  bytes=4

    // Sub-chunk 1 size.
    uint32_t Subchunk1Size;    //  bytes=4

    // Audio format.
    uint16_t AudioFormat;      //  bytes=2

    // Channels number.
    uint16_t NumChannels;      //  bytes=2

    // Sample rate.
    uint32_t SampleRate;       //  bytes=4

    // Byte rate.
    uint32_t ByteRate;         //  bytes=4

    // Block align.
    uint16_t BlockAlign;       //  bytes=2

    // Bits per sample.
    uint16_t BitsPerSample;    //  bytes=2

    // extra                            bytes=2
    // if Subchunk1Size > 16

    // Extra param size.
    uint16_t ExtraParamSize;   //  bytes=2

    // extensible                       bytes=22

    // Samples.
    uint16_t Samples;          //  bytes=2

    // Channel mask.
    uint32_t ChannelMask;      //  bytes=4

    // Sub-format GUID.
    GUID GuidSubFormat;        //  bytes=16

    // data                             bytes=8

    // Sub-chunk 2 ID.
    uint32_t Subchunk2ID;      //  bytes=4

    // Sub-chunk 2 size.
    uint32_t Subchunk2Size;    //  bytes=4

    // info

    // Extensible flag.
    bool IsExtensible;

    // Header size.
    int HeaderSize;  // normal WAV = 44 bytes, extensible WAV = 44 + 24 = 68 bytes (without extra chunks)

    // Duration in seconds.
    double Duration; // duration in seconds

    /// Total samples.
    long TotalSamples;

    // Channel types.
    static WavChannel WavChannelTypes[];

    // Multi-channel types.
    static WavChannel WavMultiChannelTypes[];

    // WAVEFORMATEXTENSIBLE sub-formats

    // Sub-type PCM.
    static GUID SubTypePCM;

    // Sub-type IEEE FLOAT.
    static GUID SubTypeIEEE_FLOAT;

    // Print formated wav file header information of this instance.
    void Print(FILE *stream)
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
};

WavChannel WavFileHeader::WavChannelTypes[] = {
    { "Mono",                    "M",   WavChannelMask::NONE },
    { "Left",                    "L",   WavChannelMask::NONE },
    { "Right",                   "R",   WavChannelMask::NONE }
};

WavChannel WavFileHeader::WavMultiChannelTypes[] = {
    { "Front Left",              "FL",   WavChannelMask::SPEAKER_FRONT_LEFT               },
    { "Front Right",             "FR",   WavChannelMask::SPEAKER_FRONT_RIGHT              },
    { "Front Center",            "FC",   WavChannelMask::SPEAKER_FRONT_CENTER             },
    { "Low Frequency",           "LF",   WavChannelMask::SPEAKER_LOW_FREQUENCY            },
    { "Back Left",               "BL",   WavChannelMask::SPEAKER_BACK_LEFT                },
    { "Back Right",              "BR",   WavChannelMask::SPEAKER_BACK_RIGHT               },
    { "Front Left of Center",    "FLC",  WavChannelMask::SPEAKER_FRONT_LEFT_OF_CENTER     },
    { "Front Right of Center",   "FRC",  WavChannelMask::SPEAKER_FRONT_RIGHT_OF_CENTER    },
    { "Back Center",             "BC",   WavChannelMask::SPEAKER_BACK_CENTER              },
    { "Side Left",               "SL",   WavChannelMask::SPEAKER_SIDE_LEFT                },
    { "Side Right",              "SR",   WavChannelMask::SPEAKER_SIDE_RIGHT               },
    { "Top Center",              "TC",   WavChannelMask::SPEAKER_TOP_CENTER               },
    { "Top Front Left",          "TFL",  WavChannelMask::SPEAKER_TOP_FRONT_LEFT           },
    { "Top Front Center",        "TFC",  WavChannelMask::SPEAKER_TOP_FRONT_CENTER         },
    { "Top Front Right",         "TFR",  WavChannelMask::SPEAKER_TOP_FRONT_RIGHT          },
    { "Top Back Left",           "TBL",  WavChannelMask::SPEAKER_TOP_BACK_LEFT            },
    { "Top Back Center",         "TBC",  WavChannelMask::SPEAKER_TOP_BACK_CENTER          },
    { "Top Back Right",          "TBR",  WavChannelMask::SPEAKER_TOP_BACK_RIGHT           }
};

// 00000001-0000-0010-8000-00aa00389b71
GUID WavFileHeader::SubTypePCM        = { 0x00000001, 0x0000, 0x0010, { 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };

// 00000003-0000-0010-8000-00aa00389b71
GUID WavFileHeader::SubTypeIEEE_FLOAT = { 0x00000003, 0x0000, 0x0010, { 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };

// Read/Write WAV file header information.
class WavFileInfo
{
private:
    static int fpeek(FILE *fs)
    {
        int c;
        c = fgetc(fs);
        ungetc(c, fs);
        return c;
    }
public:
    // Read WAV file header.
    static WavFileHeader * ReadFileHeader(FILE *fs)
    {
        WavFileHeader *h = new WavFileHeader();

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
                        byte *chunkData = new byte[chunkSize];
                        fread(chunkData, sizeof(byte), chunkSize, fs);
                        delete chunkData;

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
    // Write WAV file header.
    static void WriteFileHeader(FILE *fs, WavFileHeader *h)
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
    // Get mono WAV file header from multi-channel WAV file.
    static WavFileHeader * GetMonoWavFileHeader(WavFileHeader *h)
    {
        // Each mono output file has the same header.
        WavFileHeader *mh = new WavFileHeader();

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
};
