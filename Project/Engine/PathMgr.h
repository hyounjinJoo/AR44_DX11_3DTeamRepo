#pragma once
#include <filesystem>
namespace std
{
	namespace fs = filesystem;
}

#include "define_Res.h"


namespace mh
{
	using namespace mh::define;
	class PathMgr
	{
		friend class Application;
	public:
		static void Init();
		
	public:
		static const std::filesystem::path& GetResPathAbsolute() { return mAbsoluteResPath; }
		static const std::filesystem::path& GetResPathRelative() { return mRelativeResPath; }

		static inline std::filesystem::path GetContentPathAbsolute(eResourceType _eResType);

		static const std::filesystem::path& GetContentPathRelative(eResourceType _eResType) { return mRelativePathContent[(int)_eResType]; }

		static const std::filesystem::path& GetShaderCSOPath() { return mRelativePath_ShaderCSO; }

		static inline bool CheckExist(const std::filesystem::path& _fullPath);

	private:
		static void Release();

	private:
		static std::filesystem::path mAbsoluteResPath;
		static std::filesystem::path mRelativeResPath;
		static std::filesystem::path mRelativePathContent[(int)eResourceType::END];
		
		static std::filesystem::path mRelativePath_ShaderCSO;
	};

	inline std::filesystem::path PathMgr::GetContentPathAbsolute(eResourceType _eResType)
	{
		return mAbsoluteResPath / define::strKey::ArrResName[(int)_eResType];
	}

	inline bool PathMgr::CheckExist(const std::filesystem::path& _fullPath)
	{
		bool bRet = std::fs::exists(_fullPath);
		if (false == bRet)
		{
			std::wstring errorMsg = _fullPath.wstring();
			errorMsg += L"\n파일을 찾지 못했습니다.";
			ERROR_MESSAGE_W(errorMsg.c_str());
		}
		return bRet;
	}
}

