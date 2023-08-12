#pragma once

#include <string>

namespace mh::define
{
	struct tDesc_GPUMgr
	{
		UINT ResolutionX;
		UINT ResolutionY;
		UINT RefreshRate;
	};

	struct tDesc_Application
	{
		HWND Hwnd;
		int  LeftWindowPos;
		int	 TopWindowPos;
		int  Width;
		int  Height;
		tDesc_GPUMgr GPUDesc;
		WNDCLASSEX WinClassDesc;
	};

	struct tUmap_LightHashFunc32
	{
		UINT64 operator()(const UINT32& _ukey) const
		{
			return static_cast<UINT64>(_ukey);
		}
	};

	struct tUmap_LightHashFunc64
	{
		UINT64 operator()(const UINT64& _ukey) const
		{
			return static_cast<UINT64>(_ukey);
		}
	};

	struct tUmap_StringViewHasher
	{
		using hash_type = std::hash<std::string_view>;
		using is_transparent = void;

		std::size_t operator()(const char* str) const { return hash_type{}(str); }
		std::size_t operator()(const std::string_view str) const { return hash_type{}(str); }
		std::size_t operator()(std::string const& str) const { return hash_type{}(str); }
	};

	struct tDataPtr
	{
		void* pData;
		size_t size;

		template <typename T>
		void SetDataPtr(const T _pData) { pData = (void*)_pData; size = sizeof(T); }
	};

}
