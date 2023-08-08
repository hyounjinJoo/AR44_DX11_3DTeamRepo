#include "PCH_Contents.h"
#include "ContentsClassInitializer.h"

#include "strKey_Scene.h"
#include <Engine/SceneMgr.h>

namespace mh
{
	void ContentsClassInitializer::Init()
	{
		InitCom();
		InitScript();
		InitScene();

		InitContents();
	}


	//여기에 게임 초기화 코드를 작성
	void ContentsClassInitializer::InitContents()
	{
		SceneMgr::LoadScene(strKey::Scene::Scene_Title);
	}
}
