#include "EnginePCH.h"
#include "PathMgr.h"

namespace mh
{
	PathMgr::PathMgr()
	{
	}

	PathMgr::~PathMgr()
	{
	}

	void PathMgr::Init()
	{
		mAbsolutePath = std::filesystem::current_path();
		
		mRelativePath = ".";
		mRelativePath /= mh::define::DirName::Resource;
		
		for (int i = 0; i < (int)eResourceType::End; ++i)
		{
			mRelativePath_Res[i] = mRelativePath;
			mRelativePath_Res[i] /= mh::define::DirName::ResPath[i];
		}
	}
}

