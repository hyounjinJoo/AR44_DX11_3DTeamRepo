#include "PCH_Engine.h"
#include "PathMgr.h"

#include "AtExit.h"

namespace mh
{
	std::filesystem::path PathMgr::mAbsoluteResPath{};
	std::filesystem::path PathMgr::mRelativeResPath{};
	std::filesystem::path PathMgr::mRelativePathContent[(int)eResourceType::END]{};
	std::filesystem::path PathMgr::mRelativePath_ShaderCSO{};

	void PathMgr::Init()
	{
		AtExit::AddFunc(PathMgr::Release);

		//에러가 발생하지 않게 디렉토리가 없을 경우 생성해주는 작업까지 진행
		mAbsoluteResPath = std::filesystem::current_path().parent_path();
		mAbsoluteResPath /= define::strKey::DirName_Resource;
		if (false == std::fs::exists(mAbsoluteResPath))
		{
			std::fs::create_directories(mAbsoluteResPath);
		}

		mRelativeResPath = "../";
		mRelativeResPath /= mh::define::strKey::DirName_Resource;
		if (false == std::fs::exists(mRelativeResPath))
		{
			std::fs::create_directories(mRelativeResPath);
		}

		mRelativePath_ShaderCSO = ".";
		mRelativePath_ShaderCSO /= mh::define::strKey::DirName_CompiledShader;
		if (false == std::fs::exists(mRelativePath_ShaderCSO))
		{
			std::fs::create_directories(mRelativePath_ShaderCSO);
		}

		for (int i = 0; i < (int)eResourceType::END; ++i)
		{
			mRelativePathContent[i] = mRelativeResPath;
			mRelativePathContent[i] /= mh::define::strKey::ArrResName[i];

			if (false == std::fs::exists(mRelativePathContent[i]))
			{
				std::fs::create_directories(mRelativePathContent[i]);
			}
		}

	}

	void PathMgr::Release()
	{
		mAbsoluteResPath.clear();
		mRelativeResPath.clear();
		for (int i = 0; i < (int)eResourceType::END; ++i)
		{
			mRelativePathContent[i].clear();
		}

		mRelativePath_ShaderCSO.clear();
	}
}

