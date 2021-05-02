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

// µð¹ö±×
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#pragma comment(lib, "Msimg32.lib") // for transparnetBlt
#include <commdlg.h> // open file dialog
#include <cassert>
#include <ctime>

#include <map>
using std::map;
using std::multimap;
#include <unordered_map>
using std::unordered_map;

#include <set>
using std::set;
using std::make_pair;


#include <queue>
using std::queue;
#include <stack>
using std::stack;

#include <list>
using std::list;

#include <string>
using std::wstring;
#include <vector>
using std::vector;
#include <bitset>
using std::bitset;

#include <algorithm>

#include "Define.h"
#include "Struct.h"
#include "StringTable.h"
#include "Func.h"
#include "Tools.h"