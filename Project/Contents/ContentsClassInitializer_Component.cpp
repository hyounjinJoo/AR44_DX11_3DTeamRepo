

//=========================================================
//This Code is Automatically generated by CodeGenerator.exe
//=========================================================


#include "PCH_Contents.h"

#include "ContentsClassInitializer.h"
#include <Engine/ComMgr.h>
#include "strKey_Component.h"


#include "Com_MyRenderer.h"

#define CONSTRUCTOR_T(T) ComMgr::AddComConstructor<T>(strKey::Com::##T)

namespace mh
{
	using namespace mh::define;
	void ContentsClassInitializer::InitCom()
	{
		CONSTRUCTOR_T(Com_MyRenderer);
	}
}