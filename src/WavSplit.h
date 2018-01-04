#pragma once

#include <stdlib.h> 
#include <stdio.h>
#include <tchar.h>
#include <fcntl.h>
#include <io.h>
#include "WavFile.h"

int WavSplit(const _TCHAR *wavFilePath, const _TCHAR *outputPath);
