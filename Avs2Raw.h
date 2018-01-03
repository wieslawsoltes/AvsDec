#pragma once

#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#if defined(_WIN32) & !defined(_WIN64)
#include "avisynth\src\internal.h"
#else
#include "avisynth64\src\internal.h"
#endif

struct AvsAudioInfo
{
    int nSampleType;
    int nSamplesPerSecond;
    int nAudioChannels;
    __int64 nAudioSamples;
    int nBytesPerChannelSample;
};

struct Avs2RawStatus
{
    __int64 nStart;
    __int64 nSamples;
    __int64 nSamplesLeft;
    int nSamplesToRead;
};

class CAvs2Raw
{
private:
    bool bHaveAvisynth;
    HINSTANCE hAvisynth;
    IScriptEnvironment* (__stdcall *CreateEnv) (int);
    IScriptEnvironment *env;
    PClip *Video;
    AvsAudioInfo info;
public:
    CAvs2Raw()
    {
        Reset();
    }
    virtual ~CAvs2Raw()
    {
        CloseAvisynth();
    }
private:
    void Reset()
    {
        bHaveAvisynth = false;
        hAvisynth = nullptr;
        CreateEnv = nullptr;
        env = nullptr;
        Video = nullptr;
        memset(&info, 0, sizeof(AvsAudioInfo));
    }
public:
    bool OpenAvisynth(const char *szAvsFileName)
    {
        if (bHaveAvisynth == true)
            CloseAvisynth();

        hAvisynth = LoadLibrary(_T("avisynth"));
        if (!hAvisynth)
        {
            OutputDebugString(_T("Avisynth Error: Could not load avisynth.dll!"));
            return false;
        }

        CreateEnv = (IScriptEnvironment *(__stdcall *)(int)) GetProcAddress(hAvisynth, "CreateScriptEnvironment");
        if (!CreateEnv)
        {
            OutputDebugString(_T("Avisynth Error: Could not access CreateScriptEnvironment!"));
            return false;
        }

        env = CreateEnv(AVISYNTH_INTERFACE_VERSION);
        if (!env)
        {
            OutputDebugString(_T("Avisynth Error: Could not create scriptenvironment!"));
            return false;
        }

        AVSValue args[1] = { szAvsFileName };
        try
        {
            Video = new PClip();
            if (Video)
            {
                *Video = env->Invoke("Import", AVSValue(args, 1)).AsClip();
                if (!(*Video))
                {
                    delete Video;
                    Video = nullptr;
                    delete env;
                    env = nullptr;
                    return false;
                }
            }
            else
            {
                delete env;
                env = nullptr;
                return false;
            }
        }
        catch (AvisynthError e)
        {
            OutputDebugString(_T("Avisynth Error: Loading Avisynth script message: "));
            OutputDebugStringA(e.msg);
            delete env;
            env = nullptr;
            return false;
        }
        catch (...)
        {
            OutputDebugString(_T("Avisynth Error: Unknown error while loading Avisynth script!"));
            delete env;
            env = nullptr;
            return false;
        }

        if ((*Video)->GetVideoInfo().HasAudio())
        {
            if ((*Video)->GetVideoInfo().SampleType() != SAMPLE_FLOAT)
            {
                AVSValue args_conv[1] = { *Video };
                *Video = env->Invoke("ConvertAudioToFloat", AVSValue(args_conv, 1)).AsClip();
            }

            info.nAudioChannels = (*Video)->GetVideoInfo().AudioChannels();
            info.nAudioSamples = (*Video)->GetVideoInfo().num_audio_samples;
            info.nBytesPerChannelSample = (*Video)->GetVideoInfo().BytesPerChannelSample();
            info.nSamplesPerSecond = (*Video)->GetVideoInfo().SamplesPerSecond();
            info.nSampleType = (*Video)->GetVideoInfo().SampleType();

            bHaveAvisynth = true;
            return true;
        }
        else
        {
            OutputDebugString(_T("Avisynth Error: No audio stream!"));
            delete Video;
            Video = nullptr;
            delete env;
            env = nullptr;
            bHaveAvisynth = true;
            return false;
        }
    }
    bool CloseAvisynth()
    {
        try
        {
            if (Video)
            {
                delete Video;
                Video = nullptr;
            }

            if (env)
            {
                delete env;
                env = nullptr;
            }

            if (hAvisynth)
                FreeLibrary(hAvisynth);

            Reset();
        }
        catch (...)
        {
            OutputDebugString(_T("Avisynth Error: Failed to close Avs2Raw!"));
            return false;
        }
        return true;
    }
    AvsAudioInfo GetInputInfo()
    {
        return info;
    }
    int GetAudio(void* pBuffer, Avs2RawStatus *pStatus)
    {
        if (pStatus->nSamplesLeft <= 0)
            return 0;

        if (pStatus->nSamplesToRead > pStatus->nSamplesLeft)
            pStatus->nSamplesToRead = (int)pStatus->nSamplesLeft;

        try
        {
            (*Video)->GetAudio(pBuffer, pStatus->nStart, pStatus->nSamplesToRead, env);
        }
        catch (AvisynthError e)
        {
            OutputDebugString(_T("Avisynth Error: GetAudio() error message: "));
            OutputDebugStringA(e.msg);
            delete Video;
            delete env;
            return -1;
        }
        catch (...)
        {
            OutputDebugString(_T("Avisynth Error: Unknown error in GetAudio()!"));
            delete Video;
            delete env;
            return -1;
        }

        pStatus->nStart += pStatus->nSamplesToRead;
        pStatus->nSamplesLeft -= pStatus->nSamplesToRead;
        return pStatus->nSamplesToRead;
    }
};
