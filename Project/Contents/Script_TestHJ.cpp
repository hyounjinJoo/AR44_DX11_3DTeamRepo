#include "PCH_Contents.h"
#include "Script_TestHJ.h"
#include <Engine/InputMgr.h>
#include <Engine/GameObject.h>

namespace mh
{
	Script_TestHJ::Script_TestHJ()
		: HP_HJ(3)
	{
	}

	Script_TestHJ::~Script_TestHJ()
	{

	}

	void Script_TestHJ::Init()
	{

	}

	void Script_TestHJ::Update()
	{
		if (InputMgr::GetKey(eKeyCode::E))
		{
			GetOwner()->Destroy();
		}
	}

	int Script_TestHJ::GetHP() const
	{
		return HP_HJ;
	}

}
