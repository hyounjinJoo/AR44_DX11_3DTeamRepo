#pragma once
#include "EnginePCH.h"

#include "..\\External\\FW1FontWrapper\\Include\\FW1FontWrapper.h"
#include "..\\External\\FW1FontWrapper\\Include\\FW1CompileSettings.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\External\\FW1FontWrapper\\Library\\Debug\\FW1FontWrapperL.lib")
#else
#pragma comment(lib, "..\\External\\FW1FontWrapper\\Library\\Debug\\FW1FontWrapper.lib")
#endif

namespace mh
{
	class FontWrapper
	{
	public:
		static bool Initialize();
		static void DrawFont(const wchar_t* _string, float _x, float _y, float _size, UINT _rgb);
		static void Release();

	private:
		static IFW1Factory* mFW1Factory;
		static IFW1FontWrapper* mFontWrapper;
	};
}
