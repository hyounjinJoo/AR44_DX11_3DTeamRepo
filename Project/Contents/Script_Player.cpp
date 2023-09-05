#include "PCH_Contents.h"
#include "Script_Player.h"

#include <Engine/Animation3D.h>
#include <Engine/Com_Animator3D.h>
#include <Engine/GameObject.h>
#include <Engine/InputMgr.h>

namespace mh
{
	void Script_Player::Update()
	{
		if (InputMgr::GetKeyDown(eKeyCode::P))
		{
			Com_Animator3D* animator = GetOwner()->GetComponent<Com_Animator3D>();
			if (animator)
			{
				animator->PlayNext();
			}
		}
	}
}

