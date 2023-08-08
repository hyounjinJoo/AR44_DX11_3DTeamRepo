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
		static const std::filesystem::path& GetAbsContentPath() { return mAbsolutePath; }
		static const std::filesystem::path& GetRelContentPath() { return mRelativePath; }
		static const std::filesystem::path& GetRelResourcePath(eResourceType _eResType) { return mRelativePath_Res[(int)_eResType]; }

	private:
		static void Release();

	private:
		static std::filesystem::path mAbsolutePath;
		static std::filesystem::path mRelativePath;
		static std::filesystem::path mRelativePath_Res[(int)eResourceType::End];
	};

}

