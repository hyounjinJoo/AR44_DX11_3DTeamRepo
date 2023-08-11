#include "PCH_Client.h"
#include "guiBase.h"

#include <Engine/define_Macro.h>

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
