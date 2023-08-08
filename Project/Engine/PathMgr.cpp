#include "PCH_Engine.h"
#include "PathMgr.h"

#include "AtExit.h"

namespace mh
{
	std::filesystem::path PathMgr::mAbsolutePath;
	std::filesystem::path PathMgr::mRelativePath;
	std::filesystem::path PathMgr::mRelativePath_Res[(int)eResourceType::End];

	void PathMgr::Init()
	{
		AtExit::AddFunc(PathMgr::Release);

		mAbsolutePath = std::filesystem::current_path();
		
		mRelativePath = ".";
		mRelativePath /= mh::define::strKey::DirName_Content;
		
		for (int i = 0; i < (int)eResourceType::End; ++i)
		{
			mRelativePath_Res[i] = mRelativePath;
			mRelativePath_Res[i] /= mh::define::strKey::ArrResName[i];
		}
	}
	void PathMgr::Release()
	{
		mAbsolutePath.clear();
		mRelativePath.clear();
		for (int i = 0; i < (int)eResourceType::End; ++i)
		{
			mRelativePath_Res[i].clear();
		}
	}
}

