#include "PCH_Engine.h"
#include "ResMgr.h"

#include "AtExit.h"

namespace mh
{
	std::unordered_map<std::string, std::shared_ptr<IRes>, tUmap_StringViewHasher, std::equal_to<>> ResMgr::mArrRes[(int)eResourceType::END]{};

	void ResMgr::Init()
	{
		AtExit::AddFunc(Release);
	}

	void ResMgr::Release()
	{
		for (int i = 0; i < (int)eResourceType::END; ++i)
		{
			mArrRes[i].clear();
		}
	}
}
