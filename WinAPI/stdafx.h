#pragma once

// framework

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#pragma comment(lib, "Msimg32.lib") // for transparnetBlt
#include <cassert>
#include <ctime>

#include <unordered_map>
using std::unordered_map;
using std::make_pair;

#include <queue>
using std::queue;

#include <list>
using std::list;

#include <string>
using std::wstring;
#include <vector>
using std::vector;
#include <bitset>
using std::bitset;

#include <queue>
using std::queue;


#include "Define.h"
#include "Struct.h"
#include "StringTable.h"
#include "Func.h"