#include "PCH_Engine.h"
#include "ComMgr.h"

#include "AtExit.h"

#include "IComponent.h"

namespace mh
{
	std::unordered_map<std::string_view, std::function<IComponent* ()>> ComMgr::mUmapComConstructor;
	std::unordered_map<std::type_index, const std::string_view>         ComMgr::mUmapComName;

	IComponent* ComMgr::GetNewCom(const std::string_view _strKey)
	{
		IComponent* com = nullptr;
		const auto& iter = mUmapComConstructor.find(_strKey);
		if (iter != mUmapComConstructor.end())
		{
			com = iter->second();
			com->SetKey(iter->first);
		}
		return com;
	}

	const std::string_view ComMgr::GetComName(const std::type_info& _typeid_T_)
	{
		const auto& iter = mUmapComName.find(std::type_index(_typeid_T_));
		if (iter == mUmapComName.end())
			return "";
		return iter->second;
	}

	void ComMgr::Init()
	{
		AtExit::AddFunc(ComMgr::Release);
	}
	void ComMgr::Release()
	{
		mUmapComConstructor.clear();
		mUmapComName.clear();
	}

}

