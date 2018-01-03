#pragma once

#include "WavFile.h"

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
GUID WavFileHeader::SubTypePCM = { 0x00000001, 0x0000, 0x0010, { 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };

// 00000003-0000-0010-8000-00aa00389b71
GUID WavFileHeader::SubTypeIEEE_FLOAT = { 0x00000003, 0x0000, 0x0010, { 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };
