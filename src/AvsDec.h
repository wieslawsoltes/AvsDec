#pragma once

#define WIN32_LEAN_AND_MEAN
#include "targetver.h"
#include <windows.h>
#include <stdlib.h> 
#include <stdio.h>
#include <tchar.h>
#include "Avs2Raw.h"

int AvsDec(const _TCHAR *avsFilePath, const _TCHAR *rawFilePath);
