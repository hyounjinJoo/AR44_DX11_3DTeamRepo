#pragma once

#include "Singleton.h"
#include <filesystem>


#include "define_Res.h"


namespace mh
{
	using namespace mh::define;
	class PathMgr
		:public Singleton<PathMgr>
	{
		SINGLETON(PathMgr);

	private:
		std::filesystem::path mAbsolutePath;
		std::filesystem::path mRelativePath;
		std::filesystem::path mRelativePath_Res[(int)eResourceType::End];

	public:
		void Init();

	public:
		const std::filesystem::path& GetAbsolutePath() const { return mAbsolutePath; }
		const std::filesystem::path& GetRelativePath() const { return mRelativePath; }
		const std::filesystem::path& GetRelativeResourcePath(eResourceType _eResType) const { return mRelativePath_Res[(int)_eResType]; }
	};

}

