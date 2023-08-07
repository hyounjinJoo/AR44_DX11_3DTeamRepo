#include "PCH_Contents.h"
#include "ContentsClassInitializer.h"

#include <Engine/SceneMgr.h>
#include "strKey_Scene.h"

#include "Scene_Play.h"
#include "Scene_Title.h"

#define CONSTRUCTOR_T(T) SceneMgr::AddSceneConstructor<T>(strKey::Scene::##T)

namespace mh
{
	void ContentsClassInitializer::InitScene()
	{
		CONSTRUCTOR_T(Scene_Play);
		CONSTRUCTOR_T(Scene_Title);
	}

}
