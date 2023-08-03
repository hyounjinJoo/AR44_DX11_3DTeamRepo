#pragma once

#include <FW1FontWrapper/FW1FontWrapper.h>
#include <FW1FontWrapper/FW1CompileSettings.h>

namespace mh
{
	class FontWrapper
	{
		friend class Application;
	public:
		static void DrawFont(const wchar_t* _string, float _x, float _y, float _size, UINT _rgb);

	private:
		static bool Init();
		static void Release();

	private:
		static IFW1Factory* mFW1Factory;
		static IFW1FontWrapper* mFontWrapper;
	};
}
