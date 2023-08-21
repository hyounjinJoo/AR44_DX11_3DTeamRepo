#include "PCH_Contents.h"
#include "Scene_Example.h"

#include "strKey_Script.h"
#include <Engine/object.h>
#include <Engine/Com_Renderer_Sprite.h>
#include <Engine/ResMgr.h>

namespace mh
{
	Scene_Example::Scene_Example()
	{
	}
	Scene_Example::~Scene_Example()
	{
	}

	void Scene_Example::OnEnter()
	{
		//성큰콜로니를 출력해보려 함 -> bmp 파일 미리 로드
		//ResMgr::Load<Texture>(strKey::Texture::lurker_SunkenColony__bmp);
	}

	void Scene_Example::Init()
	{
		{
			// Main Com_Camera Game Object
			GameObject* cameraObj = object::Instantiate<GameObject>(eLayerType::Com_Camera);
			cameraObj->SetName("MainCamera");
			cameraObj->GetComponent<Com_Transform>()->SetRelativePos(float3(0.0f, 0.0f, -20.0f));
			Com_Camera* cameraComp = cameraObj->AddComponent<Com_Camera>();
			cameraComp->SetProjectionType(define::eProjectionType::Orthographic);

			//string Key만으로 AddComponent도 가능함. 이렇게 해주면 include 안해줘도 됨
			cameraObj->AddComponent(strKey::Script::Script_CameraMove);


			RenderMgr::SetMainCamera(cameraComp);
		}

		{
			//Sprite 출력 테스트
			GameObject* spriteObj = object::Instantiate<GameObject>(eLayerType::UI);
			spriteObj->SetName("SpriteTest");
			spriteObj->GetComponent<Com_Transform>()->SetRelativePos(float3(0.f, 0.f, 100.f));

			//이런 식으로 AddComponent 가능
			Com_Animator2D* animator = spriteObj->AddComponent<Com_Animator2D>();
			Com_Renderer_Sprite* spriteRenderer = spriteObj->AddComponent<Com_Renderer_Sprite>();

			//std::shared_ptr<Texture> sunken = ResMgr::Find<Texture>(strKey::Texture::lurker_SunkenColony__bmp);

			//animator->CreateXY("Sunken", sunken, 17u, 2u, 0.1f);
			//animator->Play("Sunken");
		}
	}


}
