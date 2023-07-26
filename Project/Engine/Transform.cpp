
#include "EnginePCH.h"

#include "Transform.h"
#include "Renderer.h"
#include "Camera.h"

namespace mh
{
	Transform::Transform()
		: IComponent(eComponentType::Transform)
		, mFoward(Vector3::Forward)
		, mRight(Vector3::Right)
		, mUp(Vector3::Up)
		, mScale(Vector3::One)
		, mRotation(Vector3::Zero)
		, mPosition(Vector3::One)
		, mParent(nullptr)
	{
		
	}

	Transform::~Transform()
	{
	}

	void Transform::Initialize()
	{
		//cos(180);
	}

	void Transform::Update()
	{
		//실제 로직상 캐릭터이동 처리
	}

	void Transform::FixedUpdate()
	{
		//렌더링에 사용될 위치값들을 업데이트

		// 월드 행렬 생성
		
		// 크기 변환 행렬
		Matrix scale = Matrix::CreateScale(mScale);

		// 회전 변환 행렬
		Matrix rotation;

		Vector3 radian(mRotation.x * (XM_PI / 180)
			, mRotation.y * (XM_PI / 180)
			, mRotation.z * (XM_PI / 180));

		rotation = Matrix::CreateRotationX(radian.x);
		rotation *= Matrix::CreateRotationY(radian.y);
		rotation *= Matrix::CreateRotationZ(radian.z);

		// 이동 변환 행렬
		Matrix position;
		position.Translation(mPosition);

		mWorld = scale * rotation * position;

		mFoward = Vector3::TransformNormal(Vector3::Forward, rotation);
		mRight = Vector3::TransformNormal(Vector3::Right, rotation);
		mUp = Vector3::TransformNormal(Vector3::Up, rotation);
		
		// 카메라 컴포넌트에서 세팅해준다
		// 뷰행렬 세팅
		// 프로젝션 행렬 세팅

		if (mParent)
		{
			mWorld *= mParent->mWorld;
		}
	}

	void Transform::Render()
	{
	}

	void Transform::SetConstantBuffer()
	{
		renderer::TransformCB trCb = {};
		trCb.World = mWorld;
		trCb.View = Camera::GetGpuViewMatrix();
		trCb.Projection = Camera::GetGpuProjectionMatrix();

		GPU::ConstantBuffer* cb = renderer::constantBuffers[(UINT)GPU::eCBType::Transform];
		cb->SetData(&trCb);
		cb->Bind(GPU::eShaderStage::VS);
		cb->Bind(GPU::eShaderStage::HS);
		cb->Bind(GPU::eShaderStage::DS);
		cb->Bind(GPU::eShaderStage::GS);
		cb->Bind(GPU::eShaderStage::PS);
		cb->Bind(GPU::eShaderStage::CS);
	}
}
