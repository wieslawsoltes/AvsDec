# AvsDec

[![Build status](https://ci.appveyor.com/api/projects/status/pe1bbn8xigcygoi3/branch/master?svg=true)](https://ci.appveyor.com/project/wieslawsoltes/avsdec/branch/master)

[![Github All Releases](https://img.shields.io/github/downloads/wieslawsoltes/avsdec/total.svg)](https://github.com/wieslawsoltes/AvsDec/releases)
[![GitHub Release](https://img.shields.io/github/release/wieslawsoltes/avsdec.svg)](https://github.com/wieslawsoltes/avsdec/releases/latest)
[![Github Releases](https://img.shields.io/github/downloads/wieslawsoltes/avsdec/latest/total.svg)](https://github.com/wieslawsoltes/AvsDec/releases)

Decode avisynth audio stream to raw audio file and split multi-channel WAV files into single channel WAV files.

## Download AvsDec

| Platform           | Character Set  | Version   | Download                                                                                                                       |
|--------------------|----------------|-----------|--------------------------------------------------------------------------------------------------------------------------------|
| Windows 32-bit     | Unicode        | 2.1       | [AvsDec-2.1-Win32-UNICODE.zip](https://github.com/wieslawsoltes/AvsDec/releases/download/2.1/AvsDec-2.1-Win32-UNICODE.zip)     |
| Windows 64-bit     | Unicode        | 2.1       | [AvsDec-2.1-x64-UNICODE.zip](https://github.com/wieslawsoltes/AvsDec/releases/download/2.1/AvsDec-2.1-x64-UNICODE.zip)         |
| Windows 32-bit     | MBCS           | 2.1       | [AvsDec-2.1-Win32-MBCS.zip](https://github.com/wieslawsoltes/AvsDec/releases/download/2.1/AvsDec-2.1-Win32-MBCS.zip)           |
| Windows 64-bit     | MBCS           | 2.1       | [AvsDec-2.1-x64-MBCS.zip](https://github.com/wieslawsoltes/AvsDec/releases/download/2.1/AvsDec-2.1-x64-MBCS.zip)               |

## System requirements

Minimum supported Windows version is Windows 7 SP1 or above, recommended is Windows 10 Anniversary Update.

Provided binaries should work under Windows XP Service Pack 3 (SP3) for x86, Windows XP Service Pack 2 (SP2) for x64.

Minimum supported Linux version is Ubutnu 16.10 (using Wine 2.0).

## CI Builds

[Download](https://ci.appveyor.com/project/wieslawsoltes/avsdec/build/artifacts) bleeding edge builds from the CI server.

## About

AvsDec is an audio files conversion software for decoding avisynth audio streams to raw audio files and for spliting multi-channel WAV files into single channel WAV files.

## Features

* Decode avisynth audio stream to raw audio file.
* Show wav file header info.
* Split multi-channel WAV file into single channel WAV files.
* Support for pipe output or input whenever possible.
* Full UNICODE character set support.
* Full Multibyte Character Set (MBCS) character set support.
* Full Windows 32-bit and 64-bit support.
* Works under Windows XP Service Pack 3 (SP3) for x86.
* Works under Windows XP Service Pack 2 (SP2) for x64.
* Open-source, fast, written in C++.

## Usage

```
usage: AvsDec [option] <...>
option:
        [-d] Decode avisynth audio stream to raw audio file
        examples:
        AvsDec -d <input.avs> [<output.raw>]
        AvsDec -d <input.avs> - > <output.raw>

        [-i] Show wav file header info
        examples:
        AvsDec -i <input.wav>

        [-s] Split multi-channel WAV file into single channel WAV files
        examples:
        AvsDec -s <input.wav> [<OutputPath>]
        AvsDec -s - [<OutputPath>] < <input.wav>
```

## Troubleshooting

To report issues please use [issues tracker](https://github.com/wieslawsoltes/AvsDec/issues).

## Sources

Sources are available in the [git source code repository](https://github.com/wieslawsoltes/AvsDec/).

## Build

### Prerequisites

```
git clone https://github.com/wieslawsoltes/AvsDec.git
cd AvsDec
git submodule update --init --recursive
```

### Install Visual Studio 2017

* [VS 2017](https://www.visualstudio.com/pl/downloads/)

### Windows 7 SDK

For Windows XP compatibility program is compiled using `Platform Toolset` for `Visual Studio 2017 - Windows XP (v141_xp)`.

For more details please read [Configuring Programs for Windows XP](https://msdn.microsoft.com/en-us/library/jj851139.aspx).

### Build Solution
```
Open AvsDec.sln in Visual Studios 2017 or above.
```

## License

AvsDec is licensed under the [MIT license](LICENSE.TXT).
