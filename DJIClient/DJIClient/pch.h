#pragma once

#include "targetver.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

#include <wrl.h>
#pragma comment(lib, "runtimeobject")

#define _PC_ 1

#ifndef IFR
#define IFR(hrToCheck) if (FAILED(hrToCheck)) { return hrToCheck; }
#endif



