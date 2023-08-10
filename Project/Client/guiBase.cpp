#include "PCH_Client.h"
#include "guiBase.h"

#include <Engine/define_Macro.h>

namespace gui
{
	guiBase::guiBase(const std::string_view _strKey)
		: guiEntity(_strKey)
		, mParent()
		, mChilds{}
		, mbNoChild()
		, mbEnable(true)
	{
	}

	guiBase::~guiBase()
	{
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

	guiBase* guiBase::AddChild(guiBase* _pChild)
	{
		if (mbNoChild)
		{
			SAFE_DELETE(_pChild);
		}
		else
		{
			if (_pChild)
			{
				if (_pChild->GetParent())
				{
					_pChild->GetParent()->RemoveChild(_pChild);
				}

				mChilds.push_back(_pChild);
				_pChild->SetParent(this);
			}
		}

		return _pChild;
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
