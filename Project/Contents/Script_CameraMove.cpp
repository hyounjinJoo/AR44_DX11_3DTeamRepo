#include "PCH_Contents.h"
#include "Script_CameraMove.h"

#include <Engine/GameObject.h>
#include <Engine/Com_Camera.h>
#include <Engine/Com_Transform.h>
#include <Engine/InputMgr.h>
#include <Engine/TimeMgr.h>


namespace mh
{
	Script_CameraMove::Script_CameraMove()
		: mCamera()
		, mCamSpeed(100.f)
	{
	}


	Script_CameraMove::Script_CameraMove(const Script_CameraMove& _other)
	{
	}


	Script_CameraMove::~Script_CameraMove()
	{
	}

	void Script_CameraMove::Init()
	{
		mCamera = GetOwner()->GetComponent<Com_Camera>();
	}

	void Script_CameraMove::Update()
	{
		define::eProjectionType projType = mCamera->GetProjectionType();

		switch (projType)
		{
		case mh::define::eProjectionType::None:
			MH_ASSERT(false);
			break;
		case mh::define::eProjectionType::Perspective:
			Camera3DMove();
			break;
		case mh::define::eProjectionType::Orthographic:
			Camera2DMove();
			break;
		default:
			MH_ASSERT(false);
			break;
		}
	}


	void Script_CameraMove::Camera2DMove()
	{
		Com_Transform* tf = GetOwner()->GetComponent<Com_Transform>();

		// 키 입력에 따른 카메라 이동
		float3 vPos = tf->GetPosition();

		float fSpeed = mCamSpeed;
		if (InputMgr::GetKey(eKeyCode::LSHIFT))
		{
			fSpeed *= 50000.f;
		}

		if (InputMgr::GetKey(eKeyCode::W))
		{
			vPos.y += TimeMgr::DeltaTime() * fSpeed;
		}

		if (InputMgr::GetKey(eKeyCode::S))
		{
			vPos.y -= TimeMgr::DeltaTime() * fSpeed;
		}

		if (InputMgr::GetKey(eKeyCode::A))
		{
			vPos.x -= TimeMgr::DeltaTime() * fSpeed;
		}

		if (InputMgr::GetKey(eKeyCode::D))
		{
			vPos.x += TimeMgr::DeltaTime() * fSpeed;
		}


		if (InputMgr::GetKey(eKeyCode::N_1))
		{
			float fScale = mCamera->GetScale();
			fScale += TimeMgr::DeltaTime() * 1.f;
			mCamera->SetScale(fScale);
		}

		if (InputMgr::GetKey(eKeyCode::N_2))
		{
			float fScale = mCamera->GetScale();
			fScale -= TimeMgr::DeltaTime() * 1.f;
			mCamera->SetScale(fScale);
		}

		tf->SetPosition(vPos);
	}

	void Script_CameraMove::Camera3DMove()
	{
		Com_Transform* tf = GetOwner()->GetComponent<Com_Transform>();

		float3 vPos = tf->GetPosition();
		float3 vRot = tf->GetRotation();

		float3 vFront = tf->GetForward();
		float3 vUp = tf->GetUp();
		float3 vRight = tf->GetRight();

		float fSpeed = mCamSpeed;

		if (InputMgr::GetKey(eKeyCode::LSHIFT))
			fSpeed *= 5.f;

		if (InputMgr::GetKey(eKeyCode::UP))
		{
			vPos += TimeMgr::DeltaTime() * vFront * fSpeed;
		}

		if (InputMgr::GetKey(eKeyCode::DOWN))
		{
			vPos -= TimeMgr::DeltaTime() * vFront * fSpeed;
		}

		if (InputMgr::GetKey(eKeyCode::LEFT))
		{
			vPos -= TimeMgr::DeltaTime() * vRight * fSpeed;
		}

		if (InputMgr::GetKey(eKeyCode::RIGHT))
		{
			vPos += TimeMgr::DeltaTime() * vRight * fSpeed;
		}


		if (InputMgr::GetKey(eKeyCode::RBTN))
		{
			float2 vMouseDir = InputMgr::GetMouseDir();
			vRot.y += TimeMgr::DeltaTime() * vMouseDir.x;
			vRot.x -= TimeMgr::DeltaTime() * vMouseDir.y;
		}

		tf->SetPosition(vPos);
		tf->SetRotation(float3(vRot.x, vRot.y,0.0f));
	}
}

