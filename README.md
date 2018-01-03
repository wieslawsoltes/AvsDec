# AvsDec

[![Build status](https://ci.appveyor.com/api/projects/status/pe1bbn8xigcygoi3/branch/master?svg=true)](https://ci.appveyor.com/project/wieslawsoltes/avsdec/branch/master)

Decode avisynth audio stream to raw audio file.

## Usage

```
usage: AvsDec [option] <...>
usage: AvsDec [option] <...>
option: [-d] Decode avisynth audio stream to raw audio file
        AvsDec -d <input.avs> <output.raw>
        [-i] Show wav file header info
        AvsDec -i <input.wav>
        [-s] Split multi-channel WAV file into single channel WAV files
        AvsDec -s <input.wav> [<OutputPath>]
```

## License

AvsDec is licensed under the [MIT license](LICENSE.TXT).
