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
	//Contents 프로젝트에서 건드릴수 있는 부분은 여기만임. 나머지는 터치 x
	void ContentsClassInitializer::InitContents()
	{
		//예시 씬을 첫 시작으로 등록
		SceneMgr::LoadScene(strKey::Scene::Scene_Example);
	}
}
