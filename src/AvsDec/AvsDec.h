﻿#pragma once

#define WIN32_LEAN_AND_MEAN
#include <SDKDDKVer.h>
#include <windows.h>
#include <stdlib.h> 
#include <stdio.h>
#include <tchar.h>
#include "Avs2Raw.h"

#define PRODUCTNAME     "AvsDec"
#define COPYRIGHT       "Copyright (c) Wieslaw Soltes. All rights reserved."

int AvsDec(const _TCHAR *avsFilePath, const _TCHAR *rawFilePath);
