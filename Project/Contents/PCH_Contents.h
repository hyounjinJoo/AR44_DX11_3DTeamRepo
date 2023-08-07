#ifndef PCH_CONTENTS
#define PCH_CONTENTS

#include <Engine/PCH_Engine.h>

#ifdef _DEBUG
#pragma comment(lib, "Engine/Debug/Engine.lib")
#pragma comment(lib, "Contents_CH/Debug/Contents_CH.lib")
#pragma comment(lib, "Contents_JH/Debug/Contents_JH.lib")
#pragma comment(lib, "Contents_SB/Debug/Contents_SB.lib")
#pragma comment(lib, "Contents_HJ/Debug/Contents_HJ.lib")
#else
#pragma comment(lib, "Engine/Release/Engine.lib")
#pragma comment(lib, "Contents_CH/Release/Contents_CH.lib")
#pragma comment(lib, "Contents_JH/Release/Contents_JH.lib")
#pragma comment(lib, "Contents_SB/Release/Contents_SB.lib")
#pragma comment(lib, "Contents_HJ/Release/Contents_HJ.lib")
#endif



#endif
