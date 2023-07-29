#include "EnginePCH.h"
#include "FontWrapper.h"
#ifdef _DEBUG
#pragma comment(lib, "FW1FontWrapper/Debug/FW1FontWrapperL.lib")
#else
#pragma comment(lib, "FW1FontWrapper/Release/FW1FontWrapper.lib")
#endif

#include "GPUMgr.h"

#include "AtExit.h"

namespace mh
{
	IFW1Factory* FontWrapper::mFW1Factory = nullptr;
	IFW1FontWrapper* FontWrapper::mFontWrapper = nullptr;

	bool FontWrapper::Init()
	{
		AtExit::AddFunc(Release);

		if (FAILED(FW1CreateFactory(FW1_VERSION, &mFW1Factory)))
			return false;

		auto pDevice = GPUMgr::Device();
		if (FAILED(mFW1Factory->CreateFontWrapper(pDevice.Get(), L"Arial", &mFontWrapper)))
			return false;

		return true;
	}

	void FontWrapper::DrawFont(const wchar_t* _string, float _x, float _y, float _size, UINT _rgb)
	{
		auto context = GPUMgr::Context();
		mFontWrapper->DrawString(context.Get(),
								 _string, // String
								 _size,// Font size
								 _x,// X position
								 _y,// Y position
								 _rgb,// Text color, 0xAaBbGgRr
								 0      // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
								);
	}

	void FontWrapper::Release()
	{
		mFW1Factory->Release();
		mFW1Factory = nullptr;

		mFontWrapper->Release();
		mFontWrapper = nullptr;
	}
}
