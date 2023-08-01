#include "EnginePCH.h"
#include "ResMgr.h"

#include "AtExit.h"

namespace mh
{
	std::unordered_map<std::string, std::shared_ptr<IRes>, tUmap_StringViewHasher, std::equal_to<>> ResMgr::mArrRes[(int)eResourceType::End]{};

	void ResMgr::Init()
	{
		AtExit::AddFunc(Release);
	}

	void ResMgr::Release()
	{
		for (int i = 0; i < (int)eResourceType::End; ++i)
		{
			mArrRes[i].clear();
		}

	}
}
