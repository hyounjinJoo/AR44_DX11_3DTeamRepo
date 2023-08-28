#pragma once

#include <filesystem>


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

}

