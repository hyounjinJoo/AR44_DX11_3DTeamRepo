
#include "PCH_Engine.h"

#include "Com_Transform.h"
#include "RenderMgr.h"
#include "Com_Camera.h"

#include "ConstBuffer.h"

#include "json-cpp/json.h"

namespace mh
{
	

	Com_Transform::Com_Transform()
		: IComponent(eComponentType::Transform)
		, mForward(float3::Forward)
		, mRight(float3::Right)
		, mUp(float3::Up)
		, mScale(float3::One)
		, mRotation(float3::Zero)
		, mPosition(float3::One)
	{
		//Transform은 기본적으로 생성되므로 생성자에서 Key를 지정
		SetKey(strKey::Default::com::Com_Transform);
	}


	Com_Transform::~Com_Transform()
	{
	}

	eResult Com_Transform::SaveJson(Json::Value* _pJVal)
	{
		//nullptr이면 그냥 취소
		if (nullptr == _pJVal)
		{
			return eResult::Fail_Nullptr;
		}

		//부모클래스 데이터를 저장. 실패시 리턴
		eResult result = IComponent::SaveJson(_pJVal);
		if (eResultFail(result))
		{
			return result;
		}

		//다 성공하면 json 포인터를 레퍼런스 형태로 변경
		Json::Value& jVal = *_pJVal;

		// 내꺼 저장
		//float3 mForward;
		//float3 mRight;
		//float3 mUp;
		//float3 mPosition;
		//float3 mRotation;
		//float3 mScale;
		//MATRIX mWorld;
		
		//int, uint, UINT64 등등은 기본 지원
		//우리가 직접 정의한 구조체나 float 타입은 StringConv::Convert_T_to_String 함수를 통해 변환해서 저장하면 됨
		//이 때, mForward, mRight, mUp, mWorld는 업데이트 때 새로 계산되므로 가져올 필요가 없음
		Json::MHSaveValue(_pJVal, JSON_KEY_PAIR(mPosition));
		Json::MHSaveValue(_pJVal, JSON_KEY_PAIR(mRotation));
		Json::MHSaveValue(_pJVal, JSON_KEY_PAIR(mScale));

		return eResult::Success;
	}

	eResult Com_Transform::LoadJson(const Json::Value* _pJVal)
	{
		if (nullptr == _pJVal)
		{
			return eResult::Fail_Nullptr;
		}

		//부모클래스의 LoadJson()을 호출해서 부모클래스의 데이터를 저장
		//실패시 실패결과를 리턴
		eResult result = IComponent::LoadJson(_pJVal);
		if (eResultFail(result))
		{
			return result;
		}

		Json::MHLoadValue(_pJVal, JSON_KEY_PAIR(mPosition));
		Json::MHLoadValue(_pJVal, JSON_KEY_PAIR(mRotation));

		//불러오기 실패 시 기본값으로 적용
		if (false == Json::MHLoadValue(_pJVal, JSON_KEY_PAIR(mScale)))
		{
			mScale = float3::One;
		}


		return eResult::Success;
	}

	void Com_Transform::FixedUpdate()
	{
		//렌더링에 사용될 위치값들을 업데이트

		// 월드 행렬 생성
		
		// 크기 변환 행렬
		MATRIX scale = MATRIX::CreateScale(mScale);

		// 회전 변환 행렬
		MATRIX rotation;

		float3 radian(mRotation.x * (XM_PI / 180)
			, mRotation.y * (XM_PI / 180)
			, mRotation.z * (XM_PI / 180));

		rotation = MATRIX::CreateRotationX(radian.x);
		rotation *= MATRIX::CreateRotationY(radian.y);
		rotation *= MATRIX::CreateRotationZ(radian.z);

		// 이동 변환 행렬
		MATRIX position;
		position.Translation(mPosition);

		mWorld = scale * rotation * position;

		mForward = float3::TransformNormal(float3::Forward, rotation);
		mRight = float3::TransformNormal(float3::Right, rotation);
		mUp = float3::TransformNormal(float3::Up, rotation);
		
		// 카메라 컴포넌트에서 세팅해준다
		// 뷰행렬 세팅
		// 프로젝션 행렬 세팅

		//TODO: 부모자식 구조 구현
		//if (mParent)
		//{
		//	mWorld *= mParent->mWorld;
		//}
	}


	void Com_Transform::SetConstBuffer()
	{
		TransformCB trCb = {};
		trCb.World = mWorld;
		trCb.InverseWorld = mWorld.Invert();
		trCb.View = Com_Camera::GetGpuViewMatrix();
		trCb.InverseView = trCb.View.Invert();
		trCb.Projection = Com_Camera::GetGpuProjectionMatrix();

		ConstBuffer* cb = RenderMgr::GetConstBuffer(eCBType::Transform);
		cb->SetData(&trCb);
		cb->BindData(eShaderStageFlag::ALL);
	}
}
