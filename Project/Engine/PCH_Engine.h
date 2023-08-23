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
#include <unordered_set>

#include <cmath>
#include <algorithm>
#include <limits>
#include <memory>
#include <filesystem>
#include <fstream>

#include <combaseapi.h>
#include "EnumFlags.h"

#include <PhysX/PxPhysicsAPI.h>

#ifdef _DEBUG
#pragma comment(lib, "FBXLoader/Debug/libfbxsdk-md.lib")

#pragma comment(lib, "PhysX/Debug/PhysXExtensions_static_64.lib")
#pragma comment(lib, "PhysX/Debug/PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "PhysX/Debug/PhysXVehicle_static_64.lib")
#pragma comment(lib, "PhysX/Debug/PhysXVehicle2_static_64.lib")
#pragma comment(lib, "PhysX/Debug/PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib, "PhysX/Debug/PhysXCooking_64.lib")
#pragma comment(lib, "PhysX/Debug/PVDRuntime_64.lib")
#pragma comment(lib, "PhysX/Debug/SceneQuery_static_64.lib")
#pragma comment(lib, "PhysX/Debug/PhysX_64.lib")
#pragma comment(lib, "PhysX/Debug/PhysXCommon_64.lib")
#pragma comment(lib, "PhysX/Debug/PhysXFoundation_64.lib")



#else
#pragma comment(lib, "FBXLoader/Release/libfbxsdk-md.lib")


#pragma comment(lib, "PhysX/Release/PhysXExtensions_static_64.lib")
#pragma comment(lib, "PhysX/Release/PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "PhysX/Release/PhysXVehicle_static_64.lib")
#pragma comment(lib, "PhysX/Release/PhysXVehicle2_static_64.lib")
#pragma comment(lib, "PhysX/Release/PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib, "PhysX/Release/PhysXCooking_64.lib")
#pragma comment(lib, "PhysX/Release/PVDRuntime_64.lib")
#pragma comment(lib, "PhysX/Release/SceneQuery_static_64.lib")
#pragma comment(lib, "PhysX/Release/PhysX_64.lib")
#pragma comment(lib, "PhysX/Release/PhysXCommon_64.lib")
#pragma comment(lib, "PhysX/Release/PhysXFoundation_64.lib")

#endif




namespace std
{
	namespace fs = filesystem;
}

#endif


