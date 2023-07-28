#include "EnginePCH.h"

#include "FontWrapper.h"
#include "GPUMgr.h"

#ifdef _DEBUG
#pragma comment(lib, "FW1FontWrapper/Debug/FW1FontWrapperL.lib")
#else
#pragma comment(lib, "FW1FontWrapper/Release/FW1FontWrapper.lib")
#endif

namespace mh
{
	IFW1Factory* FontWrapper::mFW1Factory = nullptr;
	IFW1FontWrapper* FontWrapper::mFontWrapper = nullptr;

	bool FontWrapper::Initialize()
	{
		if (FAILED(FW1CreateFactory(FW1_VERSION, &mFW1Factory)))
			return false;

		ID3D11Device* pDevice = GPUMgr::GetInst()->GetDevice();
		if (FAILED(mFW1Factory->CreateFontWrapper(pDevice, L"Arial", &mFontWrapper)))
			return false;

		return true;
	}

	void FontWrapper::DrawFont(const wchar_t* _string, float _x, float _y, float _size, UINT _rgb)
	{
		ID3D11DeviceContext* context = GPUMgr::GetInst()->GetContext();
		mFontWrapper->DrawString(context,
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
