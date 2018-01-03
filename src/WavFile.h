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

// WAV channel information.
class WavChannel
{
public:
    // Wav channel long name.
    const char * LongName;
    // Wav channel short name.
    const char * ShortName;
    // Wav channel mask.
    WavChannelMask Mask;
public:
    WavChannel();
    WavChannel(const char * longName, const char * shortName, WavChannelMask mask);
    virtual ~WavChannel();
};

// The canonical WAVE format starts with the RIFF header.
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
public:
    // Number of channel types.
    const static int nWavChannelTypes = 3;
    // Channel types.
    static WavChannel WavChannelTypes[nWavChannelTypes];
    // Number of multi-channel types.
    const static int nWavMultiChannelTypes = 18;
    // Multi-channel types.
    static WavChannel WavMultiChannelTypes[nWavMultiChannelTypes];
    // WAVEFORMATEXTENSIBLE sub-formats
    // Sub-type PCM.
    static GUID SubTypePCM;
    // Sub-type IEEE FLOAT.
    static GUID SubTypeIEEE_FLOAT;
public:
    // Print formated wav file header information of this instance.
    void Print(FILE *stream);
};

// Read/Write WAV file header information.
class WavFileInfo
{
private:
    static int fpeek(FILE *fs);
public:
    // Read WAV file header.
    static WavFileHeader * ReadFileHeader(FILE *fs);
    // Write WAV file header.
    static void WriteFileHeader(FILE *fs, WavFileHeader *h);
    // Get mono WAV file header from multi-channel WAV file.
    static WavFileHeader * GetMonoWavFileHeader(WavFileHeader *h);
};
