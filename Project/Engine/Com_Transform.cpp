#include "PCH_Engine.h"
#include "Com_Transform.h"

#include "GameObject.h"
#include "RenderMgr.h"
#include "Com_Camera.h"
#include "Com_RigidBody.h"

#include "ConstBuffer.h"
#include "Application.h"

#include "json-cpp/json.h"


namespace mh
{
	Com_Transform::Com_Transform()
		: mpParent(nullptr)
		, mScale(float3(1.f, 1.f, 1.f))
		, mbUpdateByMat(true)
		, mCB_Transform()
	{
	}

	Com_Transform::~Com_Transform()
	{
	}

	void Com_Transform::FixedUpdate()
	{
		
		if (true == IsPhysicsObject())
		{
			//if (true == mbUpdateByMat)
			//{
			//	GetOwner()->GetComponent<Com_RigidBody>()->SetPhysicsTransform(physx::PxTransform(mPosition, EulerToQuaternion(mRotation)));

			//	if (nullptr != mpParent)
			//		mMatWorld *= mpParent->GetWorldMatrix();

			//	return;
			//}

			physx::PxTransform transform = GetOwner()->GetComponent<Com_RigidBody>()->GetPhysicsTransform();

			auto quat = math::Quaternion::CreateFromYawPitchRoll({ XMConvertToRadians(mRelativeRotation.y),XMConvertToRadians(mRelativeRotation.x) ,XMConvertToRadians(mRelativeRotation.z) });
			math::Matrix matRotation = math::Matrix::CreateFromQuaternion(quat);
			math::Matrix matScale = math::Matrix::CreateScale(mScale);

			matRotation *= math::Matrix::CreateFromQuaternion(transform.q);
			math::Matrix matTranslation = math::Matrix::CreateTranslation(float3(transform.p) + mRelativePosition);

			mMatWorld = matScale * matRotation * matTranslation;
		}

		else
		{
			if (true == mbUpdateByMat)
			{
				if (nullptr != mpParent)
					mMatWorld *= mpParent->GetWorldMatrix();

				return;
			}

			math::Matrix matScale = math::Matrix::CreateScale(mScale);
			float4 rotateQuat = XMQuaternionRotationRollPitchYaw(
				mRotation.x * XM_PI / 180.f,
				mRotation.y * XM_PI / 180.f,
				mRotation.z * XM_PI / 180.f);

			math::Matrix matRotation = XMMatrixRotationQuaternion(rotateQuat);
			math::Matrix matTranslation = math::Matrix::CreateTranslation(mPosition);

			mMatLocal = matScale * matRotation * matTranslation;
			mMatWorld = mMatLocal;
		}

		if (nullptr != mpParent)
		{
			mMatWorld *= mpParent->GetWorldMatrix();
		}
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

	void Com_Transform::SetRotation(const float3& _rotation)
	{
		if (true == IsPhysicsObject())
		{
			Com_RigidBody* rigidBody = GetOwner()->GetComponent<Com_RigidBody>();
			physx::PxTransform transform = rigidBody->GetPhysicsTransform();

			transform.q = EulerToQuaternion(_rotation);
			rigidBody->SetPhysicsTransform(transform);
		}

		mRotation = _rotation;
	}

	void Com_Transform::SetRotation(define::eAxis3D _eAxis, float _degree)
	{
		if (true == IsPhysicsObject())
		{
			Com_RigidBody* rigidBody = GetOwner()->GetComponent<Com_RigidBody>();
			physx::PxTransform transform = rigidBody->GetPhysicsTransform();
			float3 rotVec = {};
			switch (_eAxis)
			{
			case eAxis3D::X:
				rotVec.x = 1.f;
				break;
			case eAxis3D::Y:
				rotVec.y = 1.f;
				break;
			case eAxis3D::Z:
				rotVec.z = 1.f;
				break;
			}
			physx::PxQuat q(_degree * XM_PI / 180.f, rotVec);
			rigidBody->SetPhysicsTransform(physx::PxTransform(transform.p, q));
		}

		switch (_eAxis)
		{
		case eAxis3D::X:
			mRotation.x = _degree;
			break;
		case eAxis3D::Y:
			mRotation.y = _degree;
			break;
		case eAxis3D::Z:
			mRotation.z = _degree;
			break;
		}
	}

	void Com_Transform::SetPosition(const float3& _position)
	{
		if (true == IsPhysicsObject())
		{
			Com_RigidBody* rigidBody = GetOwner()->GetComponent<Com_RigidBody>();
			physx::PxTransform transform = rigidBody->GetActor<physx::PxRigidActor>()->getGlobalPose();
			transform.p = _position;
			rigidBody->GetActor<physx::PxRigidActor>()->setGlobalPose(transform);
		}
		else
		{
			mPosition = _position;
		}
	}

	void Com_Transform::SetPosition(define::eAxis3D _eAxis, float _position)
	{
		if (true == IsPhysicsObject())
		{
			Com_RigidBody* rigidBody = GetOwner()->GetComponent<Com_RigidBody>();
			physx::PxTransform transform = rigidBody->GetDynamicActor()->getGlobalPose();

			switch (_eAxis)
			{
			case eAxis3D::X:
				transform.p.x = _position;
				break;
			case eAxis3D::Y:
				transform.p.y = _position;
				break;
			case eAxis3D::Z:
				transform.p.z = _position;
				break;
			}

			rigidBody->GetDynamicActor()->setGlobalPose(transform);
		}

		switch (_eAxis)
		{
		case eAxis3D::X:
			mPosition.x = _position;
			break;
		case eAxis3D::Y:
			mPosition.y = _position;
			break;
		case eAxis3D::Z:
			mPosition.z = _position;
			break;
		}
	}

	void Com_Transform::SetScaleFromTool(const float3& _scale)
	{
		mMatWorld._11 = _scale.x;
		mMatWorld._22 = _scale.y;
		mMatWorld._33 = _scale.z;

		DecomposeWorld();

		mScale = _scale;
	}

	void Com_Transform::SetRotationFromTool(const float3& _rotation)
	{
	}

	void Com_Transform::SetPositionFromTool(const float3& _position)
	{
	}

	void Com_Transform::SetRotationExcludingColliders(const float3& _rotation)
	{

	}

	void Com_Transform::SetRotationExcludingColliders(define::eAxis3D _eAxis, float _degree)
	{
	}

	void Com_Transform::SetPositionExcludingColliders(const float3& _position)
	{
		mRelativePosition = _position;
	}

	void Com_Transform::SetPositionExcludingColliders(define::eAxis3D _eAxis, float _position)
	{
		switch (_eAxis)
		{
		case eAxis3D::X:
			mRelativePosition.x = _position;
			break;
		case eAxis3D::Y:
			mRelativePosition.y = _position;
			break;
		case eAxis3D::Z:
			mRelativePosition.z = _position;
			break;
		}
	}

	void Com_Transform::SetWorldMatrix(const math::Matrix& _matrix)
	{
		mbUpdateByMat = true;
		mMatWorld = _matrix;
		DecomposeWorld();
	}

	void Com_Transform::DecomposeWorld()
	{
		XMVECTOR vScale;
		XMVECTOR vRotQ;
		XMVECTOR vPos;
		XMMatrixDecompose(&vScale, &vRotQ, &vPos, mMatWorld);


		mScale = vScale;
		mPosition = vPos;

		// Quaternion to Euler Angle
		// 출처 http://www.littlecandle.co.kr/bbs/board.php?bo_table=codingnote&wr_id=174&page=2
		float w, x, y, z;
		w = vRotQ.m128_f32[3];
		x = vRotQ.m128_f32[0];
		y = vRotQ.m128_f32[1];
		z = vRotQ.m128_f32[2];

		float sqW = w * w;
		float sqX = x * x;
		float sqY = y * y;
		float sqZ = z * z;
		float unit = sqX + sqY + sqZ + sqW;
		float test = x * w - y * z;
		float3 v;

		if (test > 0.4955f * unit)
		{
			v.y = 2.f * atan2f(y, x);
			v.x = XM_PI / 2.f;
			v.z = 0;
			v = v * (180.f / XM_PI);
		}
		else if (test < -0.4995f * unit)
		{
			v.x = -2.f * atan2f(y, x);
			v.x = -XM_PI / 2.f;
			v.z = 0;
			v = v * (180.f / XM_PI);
		}
		else
		{
			float4 Quat(w, z, x, y);
			v.x = (float)asinf(2.f * (Quat.x * Quat.z - Quat.w * Quat.y)); // Pitch
			v.y = (float)atan2(2.f * Quat.x * Quat.w + 2.f * Quat.y * Quat.z, 1.f - 2.f * (Quat.z * Quat.z + Quat.w * Quat.w)); // Yaw
			v.z = (float)atan2(2.f * Quat.x * Quat.y + 2.f * Quat.z * Quat.w, 1.f - 2.f * (Quat.y * Quat.y + Quat.z * Quat.z)); // Roll
			v = v * (180.f / XM_PI);
		}

		mRotation = NormalizeAngles(v);
	}

	float Com_Transform::NormalizeAngle(float _angle)
	{
		while (_angle > 360)
			_angle -= 360;
		while (_angle < 0)
			_angle += 360;
		return _angle;
	}

	float3 Com_Transform::NormalizeAngles(float3 _angles)
	{
		_angles.x = NormalizeAngle(_angles.x);
		_angles.y = NormalizeAngle(_angles.y);
		_angles.z = NormalizeAngle(_angles.z);
		return _angles;
	}

	float3 Com_Transform::QuaternionToEuler(float _x, float _y, float _z, float _w)
	{
		float w = _w, x = _x, y = _y, z = _z;

		float sqW = w * w;
		float sqX = x * x;
		float sqY = y * y;
		float sqZ = z * z;
		float unit = sqX + sqY + sqZ + sqW;
		float test = x * w - y * z;
		float3 v;

		if (test > 0.4955f * unit)
		{
			v.y = 2.f * atan2f(y, x);
			v.x = XM_PI / 2.f;
			v.z = 0;
			v = v * (180.f / XM_PI);
		}
		else if (test < -0.4995f * unit)
		{
			v.x = -2.f * atan2f(y, x);
			v.x = -XM_PI / 2.f;
			v.z = 0;
			v = v * (180.f / XM_PI);
		}
		else
		{
			float4 Quat(w, z, x, y);
			v.x = (float)asinf(2.f * (Quat.x * Quat.z - Quat.w * Quat.y)); // Pitch
			v.y = (float)atan2(2.f * Quat.x * Quat.w + 2.f * Quat.y * Quat.z, 1.f - 2.f * (Quat.z * Quat.z + Quat.w * Quat.w)); // Yaw
			v.z = (float)atan2(2.f * Quat.x * Quat.y + 2.f * Quat.z * Quat.w, 1.f - 2.f * (Quat.y * Quat.y + Quat.z * Quat.z)); // Roll
			v = v * (180.f / XM_PI);
		}

		NormalizeAngles(v);
		return v;
	}

	physx::PxQuat Com_Transform::EulerToQuaternion(float3 _euler)
	{
		float4 rotateQuat = XMQuaternionRotationRollPitchYaw(
			_euler.x * XM_PI / 180.f,
			_euler.y * XM_PI / 180.f,
			_euler.z * XM_PI / 180.f);

		return { rotateQuat.x, rotateQuat.y, rotateQuat.z, rotateQuat.w };
	}

	float3 Com_Transform::GetWorldPosition()
	{
		float3 position = mPosition;

		if (nullptr != mpParent)
			position += mpParent->GetWorldPosition();

		return position;
	}

	const float3& Com_Transform::GetPosition()
	{
		return mPosition;
	}

	const float3& Com_Transform::GetRotation()
	{
		return mRotation;
	}

	const float3& Com_Transform::GetScale()
	{
		return mScale;
	}

	

	void Com_Transform::AddRotation(define::eAxis3D _eAxis, float _degree)
	{
		if (true == IsPhysicsObject())
		{
			Com_RigidBody* rigidBody = GetOwner()->GetComponent<Com_RigidBody>();
			physx::PxTransform transform = rigidBody->GetPhysicsTransform();
			float3 rotVec = {};
			switch (_eAxis)
			{
			case eAxis3D::X:
				rotVec.x = 1.f;
				break;
			case eAxis3D::Y:
				rotVec.y = 1.f;
				break;
			case eAxis3D::Z:
				rotVec.z = 1.f;
				break;
			}
			physx::PxQuat q = transform.q;
			physx::PxQuat rotQ(_degree * XM_PI / 180.f, rotVec);
			rigidBody->SetPhysicsTransform(physx::PxTransform(transform.p, q * rotQ));
		}

		switch (_eAxis)
		{
		case eAxis3D::X:
			mRotation.x += _degree;
			break;
		case eAxis3D::Y:
			mRotation.y += _degree;
			break;
		case eAxis3D::Z:
			mRotation.z += _degree;
			break;
		}
	}

	void Com_Transform::Move(const float3& _velocity)
	{
		if (true == IsPhysicsObject())
		{
			Com_RigidBody* rigidBody = GetOwner()->GetComponent<Com_RigidBody>();
			physx::PxTransform transform = rigidBody->GetPhysicsTransform();
			transform.p += _velocity * TimeMgr::DeltaTime();
			define::eActorType eActorType = rigidBody->GetActorType();

			if (define::eActorType::Kinematic == eActorType)
				rigidBody->GetDynamicActor()->setKinematicTarget(transform);
			else if (define::eActorType::Dynamic == eActorType)
				rigidBody->GetDynamicActor()->setGlobalPose(transform);
			else
				AssertEx(false, L"Transform::Move() - Static Actor에 대한 Move 호출");
		}

		else
		{
			mPosition += _velocity * TimeMgr::DeltaTime();
		}
	}
	void Com_Transform::BindData()
	{
		//mCB_Transform = {};

		mCB_Transform.world = mMatWorld;
		
		mCB_Transform.view = Com_Camera::GetGpuViewMatrix();
		mCB_Transform.inverseView = Com_Camera::GetGpuViewInvMatrix();
		mCB_Transform.projection = Com_Camera::GetGpuProjectionMatrix();
		mCB_Transform.WorldView = mCB_Transform.world * mCB_Transform.view;
		mCB_Transform.WVP = mCB_Transform.WorldView * mCB_Transform.projection;

		ConstBuffer* cb = RenderMgr::GetConstBuffer(eCBType::Transform);
		cb->SetData(&mCB_Transform);
		cb->BindData(eShaderStageFlag::ALL);
	}
}
