#ifndef PCH_ENGINE
#define PCH_ENGINE

#include <string>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <assert.h>

#include <array>
#include <vector>
#include <list>
#include <map>
#include <bitset>
#include <set>
#include <functional>

#include <cmath>
#include <algorithm>
#include <limits>
#include <memory>
#include <filesystem>
#include <fstream>

#include <combaseapi.h>
#include "EnumFlags.h"

#ifdef _DEBUG
#pragma comment(lib, "FBXLoader/Debug/libfbxsdk-md.lib")
#else
#pragma comment(lib, "FBXLoader/Release/libfbxsdk-md.lib")
#endif

#endif


