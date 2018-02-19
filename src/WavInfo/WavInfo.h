#pragma once

#include <stdlib.h> 
#include <stdio.h>
#include <tchar.h>
#include "WavFile.h"

#define PRODUCTNAME     "WavInfo"
#define COPYRIGHT       "Copyright (c) Wieslaw Soltes. All rights reserved."

int WavInfo(const _TCHAR *wavFilePath);
