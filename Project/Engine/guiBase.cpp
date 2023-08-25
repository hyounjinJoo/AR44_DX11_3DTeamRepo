#include "PCH_Engine.h"
#include "guiBase.h"

#include "define_Macro.h"

#include "json-cpp/json.h"

namespace gui
{
	guiBase::guiBase(const std::string_view _strName)
		: guiEntity(_strName)
		, mParent()
		, mChilds{}
		, mbNoChild()
		, mbEnable(true)
	{
	}

	guiBase::~guiBase()
	{
		for (size_t i = 0; i < mChilds.size(); ++i)
		{
			if (mChilds[i])
				delete mChilds[i];
		}
	}
	mh::eResult guiBase::SaveJson(Json::Value* _pJval)
	{
		if (nullptr == _pJval)
			return mh::eResult::Fail_Nullptr;

		mh::eResult result = guiEntity::SaveJson(_pJval);
		if (eResultFail(result))
			return result;

		Json::MH::SaveValue(_pJval, JSON_KEY_PAIR(mbEnable));

		for (size_t i = 0; i < mChilds.size(); ++i)
		{
			if (mChilds[i])
				mChilds[i]->SaveJson(_pJval);
		}

		return mh::eResult::Success;
	}
	mh::eResult guiBase::LoadJson(const Json::Value* _pJval)
	{
		if (nullptr == _pJval)
			return mh::eResult::Fail_Nullptr;

		mh::eResult result = guiEntity::LoadJson(_pJval);
		if (eResultFail(result))
			return result;

		Json::MH::LoadValue(_pJval, JSON_KEY_PAIR(mbEnable));

		for (size_t i = 0; i < mChilds.size(); ++i)
		{
			if (mChilds[i])
				mChilds[i]->LoadJson(_pJval);
		}

		return mh::eResult::Success;
	}
	void guiBase::InitRecursive()
	{
		Init();
		for (size_t i = 0; i < mChilds.size(); ++i)
		{
			mChilds[i]->InitRecursive();
		}
	}
	void guiBase::FixedUpdate()
	{
		if (false == GetEnable())
			return;

		//자신의 고유 Update 항목을 호출
		Update();

		if (true == BeginUI())
		{
			UpdateUI();
			for (size_t i = 0; i < mChilds.size(); ++i)
			{
				mChilds[i]->FixedUpdate();
			}

			EndUI();
		}
	}

	void guiBase::AddChild(guiBase* _pChild)
	{
		if (_pChild)
		{
			_pChild->MakeUniqueKeyByName();
			_pChild->SetParent(this);
			mChilds.push_back(_pChild);
		}
	}


	void guiBase::RemoveChild(guiBase* _pChild)
	{
		for (auto iter = mChilds.begin(); iter != mChilds.end(); ++iter)
		{
			if ((*iter) == _pChild)
			{
				(*iter)->SetParent(nullptr);
				mChilds.erase(iter);
				break;
			}
		}
	}
}
