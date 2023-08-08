#include "PCH_Contents.h"
#include "Script_JH.h"

#include <Engine/GameObject.h>
#include "Script_TestHJ.h"

mh::Script_JH::Script_JH()
	: TEST_JH()
{
}

mh::Script_JH::~Script_JH()
{
}

void mh::Script_JH::Init()
{
	TEST_JH = GetOwner()->GetComponent<Script_TestHJ>()->GetHP();
}
