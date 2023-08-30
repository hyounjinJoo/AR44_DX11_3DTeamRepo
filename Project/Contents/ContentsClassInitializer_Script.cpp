

//=========================================================
//This Code is Automatically generated by CodeGenerator.exe
//=========================================================


#include "PCH_Contents.h"

#include "ContentsClassInitializer.h"
#include <Engine/ComMgr.h>
#include "strKey_Script.h"


#include "Script_CameraMove.h"
#include "Script_UIBase.h"
#include "Script_UIButton.h"
#include "Script_UIGauge.h"

#define CONSTRUCTOR_T(T) ComMgr::AddComConstructor<T>(strKey::Script::##T)

namespace mh
{
	using namespace mh::define;
	void ContentsClassInitializer::InitScript()
	{
		CONSTRUCTOR_T(Script_CameraMove);
		CONSTRUCTOR_T(Script_UIBase);
		CONSTRUCTOR_T(Script_UIButton);
		CONSTRUCTOR_T(Script_UIGauge);
	}
}