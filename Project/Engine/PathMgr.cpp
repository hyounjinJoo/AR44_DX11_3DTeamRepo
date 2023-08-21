#include "PCH_Engine.h"
#include "PathMgr.h"

#include "AtExit.h"

namespace mh
{
	std::filesystem::path PathMgr::mAbsolutePath{};
	std::filesystem::path PathMgr::mRelativePath{};
	std::filesystem::path PathMgr::mRelativePath_Res[(int)eResourceType::END]{};
	std::filesystem::path PathMgr::mRelativePath_ShaderCSO{};

	void PathMgr::Init()
	{
		AtExit::AddFunc(PathMgr::Release);

		//에러가 발생하지 않게 디렉토리가 없을 경우 생성해주는 작업까지 진행

		mAbsolutePath = std::filesystem::current_path();
		mAbsolutePath /= define::strKey::DirName_Content;
		if (false == std::fs::exists(mAbsolutePath))
		{
			std::fs::create_directories(mAbsolutePath);
		}

		mRelativePath = ".";
		mRelativePath /= mh::define::strKey::DirName_Content;
		if (false == std::fs::exists(mRelativePath))
		{
			std::fs::create_directories(mRelativePath);
		}

		mRelativePath_ShaderCSO = ".";
		mRelativePath_ShaderCSO /= mh::define::strKey::DirName_ShaderCSO;
		if (false == std::fs::exists(mRelativePath_ShaderCSO))
		{
			std::fs::create_directories(mRelativePath_ShaderCSO);
		}

		for (int i = 0; i < (int)eResourceType::END; ++i)
		{
			mRelativePath_Res[i] = mRelativePath;
			mRelativePath_Res[i] /= mh::define::strKey::ArrResName[i];

			if (false == std::fs::exists(mRelativePath_Res[i]))
			{
				std::fs::create_directories(mRelativePath_Res[i]);
			}
		}

	}

	void PathMgr::Release()
	{
		mAbsolutePath.clear();
		mRelativePath.clear();
		for (int i = 0; i < (int)eResourceType::END; ++i)
		{
			mRelativePath_Res[i].clear();
		}

		mRelativePath_ShaderCSO.clear();
	}
}

