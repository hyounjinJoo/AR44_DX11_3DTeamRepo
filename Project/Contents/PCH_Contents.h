#ifndef PCH_CONTENTS
#define PCH_CONTENTS

#include <Engine/PCH_Engine.h>

#ifdef _DEBUG
#pragma comment(lib, "Engine/Debug/Engine.lib")
#else
#pragma comment(lib, "Engine/Release/Engine.lib")
#endif



#endif
