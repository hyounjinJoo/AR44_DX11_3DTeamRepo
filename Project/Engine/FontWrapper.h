#pragma once

#include "SimpleMath.h"
#include <FW1FontWrapper/FW1FontWrapper.h>
#include <FW1FontWrapper/FW1CompileSettings.h>

struct IFW1Factory;
struct IFW1FontWrapper;

namespace mh
{

	class FontWrapper
	{
		friend class Application;
	public:
		static void DrawFont(const wchar_t* _string, float _x, float _y, float _size, uint _rgb);

	private:
		static bool Init();
		static void Release();

	private:
		static IFW1Factory* mFW1Factory;
		static IFW1FontWrapper* mFontWrapper;
	};
}
