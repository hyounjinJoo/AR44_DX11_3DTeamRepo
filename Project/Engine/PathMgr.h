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
		static const std::filesystem::path& GetResPathAbsolute() { return mAbsolutePath; }
		static const std::filesystem::path& GetResPathRelative() { return mRelativePath; }

		static inline std::filesystem::path GetContentPathAbsolute(eResourceType _eResType);

		static const std::filesystem::path& GetContentPathRelative(eResourceType _eResType) { return mRelativePath_Res[(int)_eResType]; }

		static const std::filesystem::path& GetShaderCSOPath() { return mRelativePath_ShaderCSO; }

	private:
		static void Release();

	private:
		static std::filesystem::path mAbsolutePath;
		static std::filesystem::path mRelativePath;
		static std::filesystem::path mRelativePath_Res[(int)eResourceType::END];
		
		static std::filesystem::path mRelativePath_ShaderCSO;
	};

	inline std::filesystem::path PathMgr::GetContentPathAbsolute(eResourceType _eResType)
	{
		return mAbsolutePath / define::strKey::ArrResName[(int)_eResType];
	}

}

