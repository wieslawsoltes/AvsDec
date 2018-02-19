#pragma once

#include <memory>
#include <vector>
#include <stdlib.h> 
#include <stdio.h>
#include <tchar.h>
#include <fcntl.h>
#include <io.h>
#include "WavFile.h"

#define PRODUCTNAME     "WavSplit"
#define COPYRIGHT       "Copyright (c) Wieslaw Soltes. All rights reserved."

int WavSplit(const _TCHAR *wavFilePath, const _TCHAR *outputPath);
