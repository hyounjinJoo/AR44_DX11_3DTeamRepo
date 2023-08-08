#include "PCH_Engine.h"
#include "Com_Transform.h"

#include "GameObject.h"
#include "RenderMgr.h"
#include "Com_Camera.h"

#include "ConstBuffer.h"
#include "Application.h"

#include "json-cpp/json.h"


namespace mh
{

	Com_Transform::Com_Transform()
		: IComponent(eComponentType::Transform)
		, mSize(100.f, 100.f, 100.f)
		, mScaleRelative(1.f, 1.f, 1.f)
		, mIsScaleDefault(true)
		, mbInheritScale(true)
		, mbInheritRot(true)
		, mbSizeUpdated(true)
		, mbLockRotation()
		, mbNeedMyUpdate(true)
	{
		SetKey(strKey::Default::com::Com_Transform);
	}

	Com_Transform::~Com_Transform()
	{
	}

	void Com_Transform::Init()
	{
	}

	void Com_Transform::Update()
	{
	}

	void Com_Transform::FixedUpdate()
	{
		//bool 값들은 Tick()에서 false로 초기화 된다.
		//여기선 고유 크기(Size)를 반영하지 않은 월드행렬을 만든다.
		//게임오브젝트 상속 관계에서 고유 크기까지 상속을 받게 되면 기하급수적으로 크기가 커짐 
		if (true == mbNeedMyUpdate)
		{
			//자신의 트랜스폼 업데이트를 진행할 경우 - 두개 다 업데이트 해줘야함.
			UpdateMyTransform();
		}

		//부모 트랜스폼'만' 갱신되었을 경우 : 자신은 갱신할 필요 없음.
		GameObject* parent = GetOwner()->GetParent();
		if (parent && parent->GetTransform().IsUpdated())
		{
			UpdateParentMatrix();
		}

		bool Updated = false;
		//둘중에 하나라도 업데이트 되었을 경우 월드행렬을 새로 계산한다.
		if (mbNeedMyUpdate || mbSizeUpdated)
		{
			Updated = true;

			//부모 행렬이 있을 경우 부모행렬을 곱해줌.
			if (GetOwner()->GetParent())
				mMatWorldWithoutSize = mMatRelative * mMatParent;
			else
				mMatWorldWithoutSize = mMatRelative;

			//자신의 사이즈가 반영된 최종 월드행렬을 계산
			mMatWorldFinal = MATRIX::CreateScale(mSize) * mMatWorldWithoutSize;
		}


		Application::AddDestroyFunc(std::bind(&Com_Transform::ClearUpdateState, this));
	}

	void Com_Transform::Render()
	{
	}

	eResult Com_Transform::SaveJson(Json::Value* _pJson)
	{
		return eResult::Fail_NotImplemented;

		if (nullptr == _pJson)
			return eResult::Fail_Nullptr;

		//Com_Transform 항목을 하나 만들어서 그 안에 저장
		Json::Value& jVal = (*_pJson);

		//{//사이즈 X, Y, Z 순서로 저장
		//	std::string strKey = std::string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::v3Size);
		//	jVal[strKey] = Json::Value(Json::ValueType::arrayValue);
		//	jVal[strKey].append(Pack_float_int(mSize.x).i);
		//	jVal[strKey].append(Pack_float_int(mSize.y).i);
		//	jVal[strKey].append(Pack_float_int(mSize.z).i);
		//}

		//{
		//	std::string strKey = std::string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::v3RelativeScale);
		//	jVal[strKey] = Json::Value(Json::ValueType::arrayValue);
		//	jVal[strKey].append(Pack_float_int(mScaleRelative.x).i);
		//	jVal[strKey].append(Pack_float_int(mScaleRelative.y).i);
		//	jVal[strKey].append(Pack_float_int(mScaleRelative.z).i);
		//}

		//{
		//	std::string strKey = std::string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::v3RelativePos);
		//	jVal[strKey] = Json::Value(Json::ValueType::arrayValue);
		//	jVal[strKey].append(Pack_float_int(m_v3RelativePos.x).i);
		//	jVal[strKey].append(Pack_float_int(m_v3RelativePos.y).i);
		//	jVal[strKey].append(Pack_float_int(m_v3RelativePos.z).i);
		//}

		//{
		//	std::string strKey = std::string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::v3RelativeRot);
		//	jVal[strKey] = Json::Value(Json::ValueType::arrayValue);
		//	jVal[strKey].append(Pack_float_int(m_v3RelativeRot.x).i);
		//	jVal[strKey].append(Pack_float_int(m_v3RelativeRot.y).i);
		//	jVal[strKey].append(Pack_float_int(m_v3RelativeRot.z).i);
		//}

		//{
		//	jVal[std::string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::bInheritRot)] = mbInheritRot;
		//	jVal[std::string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::bInheritScale)] = mbInheritScale;
		//	jVal[std::string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::bLockRot)] = mbLockRotation;
		//}

		return eResult::Success;
	}

	eResult Com_Transform::LoadJson(const Json::Value* _pJson)
	{
		return eResult::Fail_NotImplemented;

		if (nullptr == _pJson)
			return eResult::Fail_Nullptr;

		//Json::Value& jVal = (*_pJson);

		//auto ReadVec3FromJson = [](const Json::Value& _jVal, const std::string& _strKey, float3& _outVal)->bool
		//	{
		//		if (_jVal.isMember(_strKey))
		//		{
		//			if (_jVal[_strKey].size() != 3)
		//				return false;

		//			_outVal.x = Pack_float_int(_jVal[_strKey][0].asInt()).f;
		//			_outVal.y = Pack_float_int(_jVal[_strKey][1].asInt()).f;
		//			_outVal.z = Pack_float_int(_jVal[_strKey][2].asInt()).f;

		//			return true;
		//		}
		//		else
		//			return false;
		//	};



		//{//RelativeSize
		//	std::string strKey = std::string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::v3Size);
		//	float3 size;
		//	if (ReadVec3FromJson(jVal, strKey, size))
		//	{
		//		SetSize(size);
		//	}
		//	else
		//		return false;
		//}


		//{//RelativeScale
		//	std::string strKey = std::string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::v3RelativeScale);
		//	float3 scale;
		//	if (ReadVec3FromJson(jVal, strKey, scale))
		//	{
		//		SetRelativeScale(scale);
		//	}
		//	else
		//		return false;
		//}

		//{//RelativePos
		//	std::string strKey = std::string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::v3RelativePos);
		//	float3 pos;
		//	if (ReadVec3FromJson(jVal, strKey, pos))
		//	{
		//		SetRelativePos(pos);
		//	}
		//	else
		//		return false;
		//}

		//{//RelativeRot
		//	std::string strKey = std::string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::v3RelativeRot);
		//	float3 rot;
		//	if (ReadVec3FromJson(jVal, strKey, rot))
		//	{
		//		SetRelativeRot(rot);
		//	}
		//	else
		//		return false;
		//}


		//{
		//	std::string strKey = std::string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::bInheritRot);
		//	if (jVal.isMember(strKey))
		//		mbInheritRot = jVal[strKey].asBool();
		//	else return false;

		//	strKey = std::string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::bInheritScale);
		//	if (jVal.isMember(strKey))
		//		mbInheritScale = jVal[strKey].asBool();
		//	else return false;

		//	strKey = std::string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::bLockRot);
		//	if (jVal.isMember(strKey))
		//	{
		//		mbLockRotation = jVal[strKey].asBool();
		//	}
		//	else return false;
		//}


		return eResult::Success;
	}



	void Com_Transform::UpdateMyTransform()
	{
		mMatRelative = MATRIX::Identity;

		//1. 크기행렬
		if (false == mIsScaleDefault)
		{
			//크기행렬(CreateScale을 해주면 자동으로 동차좌표를 추가해서 행렬에 삽입해 준다.
			mMatRelative *= MATRIX::CreateScale(mScaleRelative);
		}

		//2. 회전행렬
		MATRIX matRot = MATRIX::CreateFromQuaternion(math::Quaternion::CreateFromYawPitchRoll(mRotRelative.y, mRotRelative.x, mRotRelative.z));
		//회전행렬으로부터 직관적 방향을 계산한다.
		mDirRelative[(UINT)eDirectionType::FRONT] = matRot.Forward();
		mDirRelative[(UINT)eDirectionType::RIGHT] = matRot.Right();
		mDirRelative[(UINT)eDirectionType::UP] = matRot.Up();
		//방향은 쿼터니언을 사용해서 계산.

		//회전 잠금 상태가 아닐 경우에만 회전행렬을 곱해준다.
		if (false == mbLockRotation)
			mMatRelative *= matRot;

		//3. 이동행렬
		mMatRelative *= MATRIX::CreateTranslation(mPosRelative);
	}


	void Com_Transform::UpdateParentMatrix()
	{
		mMatParent = MATRIX::Identity;

		GameObject* parent = GetOwner()->GetParent();
		if (nullptr == parent)
		{
			return;
		}

		mMatParent = parent->GetTransform().GetWorldMatWithoutSize();

		//부모 오브젝트가 있을 경우 부모의 월드행렬을 받아온다. 
		//성공 시 true가 반환되므로 이 때는 상속 과정을 시작하면 됨
		bool bWorldDirInherit = false;
		if (true == mbInheritRot)
		{
			//회전 상속 + 크기 미상속 -> 크기정보 제거
			if (false == mbInheritScale)
			{
				//정규화해서 크기정보를 제거
				mMatParent.Right(mMatParent.Right().Normalize());
				mMatParent.Up(mMatParent.Up().Normalize());
				mMatParent.Forward(mMatParent.Forward().Normalize());
			}
			//else: 둘 다 상속 받는 경우에는 작업할 것이 없음. 그냥 빠져나가면 됨

			bWorldDirInherit = true;	//이때만 월드방향을 상속받아주면 된다.
		}
		else
		{
			constexpr size_t eraseSize = sizeof(float) * 12;
			//회전 미상속 + 크기 상속 -> 회전정보 제거
			if (true == mbInheritScale)
			{
				//회전정보만 상속받는 경우: 크기정보만 추출
				float3 Scale(mMatParent.Right().Length(), mMatParent.Up().Length(), mMatParent.Forward().Length());
				//float(4) * 12 -> 회전 파트를 모두 0으로 밀어버리고 크기만 등록

				memset(mMatParent.m, 0, eraseSize);
				mMatParent._11 = Scale.x;
				mMatParent._22 = Scale.y;
				mMatParent._33 = Scale.z;
			}
			//회전 미상속 + 크기 미상속 -> 전부 밀고 단위행렬로
			else
			{
				memset(mMatParent.m, 0, eraseSize);
				mMatParent._11 = 1.f;
				mMatParent._22 = 1.f;
				mMatParent._33 = 1.f;
			}
		}
	}


	void Com_Transform::SetConstBuffer()
	{
		TransformCB trCb = {};
		trCb.World = mMatWorldFinal;
		trCb.InverseWorld = mMatWorldFinal.Invert();
		trCb.View = Com_Camera::GetGpuViewMatrix();
		trCb.InverseView = trCb.View.Invert();
		trCb.Projection = Com_Camera::GetGpuProjectionMatrix();

		ConstBuffer* cb = RenderMgr::GetConstBuffer(eCBType::Transform);
		cb->SetData(&trCb);
		cb->BindData(eShaderStageFlag::ALL);
	}
}
