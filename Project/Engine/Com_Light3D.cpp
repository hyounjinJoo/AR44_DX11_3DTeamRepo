#include "PCH_Engine.h"

#include "Com_Light3D.h"
#include "Com_Transform.h"
#include "GameObject.h"
#include "RenderMgr.h"
#include "ResMgr.h"
#include "Mesh.h"
#include "Material.h"

#include "ConstBuffer.h"

#include "json-cpp/json.h"

namespace mh
{
	Com_Light3D::Com_Light3D()
		: ILight(eDimensionType::_3D)
		, mVolumeMesh()
		, mLightMaterial()
		, mIndex()
	{
		RenderMgr::AddLight(this);

	}

	Com_Light3D::Com_Light3D(const Com_Light3D& _other)
		: ILight(_other)
		, mIndex(_other.mIndex)
		, mAttribute(_other.mAttribute)
		, mVolumeMesh(_other.mVolumeMesh)
		, mLightMaterial(_other.mLightMaterial)
	{
		RenderMgr::AddLight(this);
	}

	Com_Light3D::~Com_Light3D()
	{
		RenderMgr::RemoveLight(this);
	}

	eResult Com_Light3D::SaveJson(Json::Value* _pJVal)
	{
		if (nullptr == _pJVal)
		{
			return eResult::Fail_Nullptr;
		}

		eResult result = IComponent::SaveJson(_pJVal);
		if (eResultFail(result))
		{
			return result;
		}

		Json::Value& jVal = *_pJVal;

		Json::MH::SaveValue(_pJVal, JSON_KEY_PAIR(mAttribute));

		return eResult::Success;
	}

	eResult Com_Light3D::LoadJson(const Json::Value* _pJVal)
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
	
		Json::MH::LoadValue(_pJVal, JSON_KEY_PAIR(mAttribute));

		SetType((eLightType)mAttribute.lightType);

		//불러오기 실패 시 기본값으로 적용
		if (false == Json::MH::LoadValue(_pJVal, JSON_KEY_PAIR(mAttribute.lightType)))
		{
			mAttribute.lightType = (int)eLightType::Directional;
		}

		return eResult::Success;
	}

	void Com_Light3D::FixedUpdate()
	{
		Com_Transform* tr = GetOwner()->GetComponent<Com_Transform>();

		if (eLightType::Point == (eLightType)mAttribute.lightType)
		{
			tr->SetRelativeScale(float3(mAttribute.radius * 5.f));
		}

		float3 position = tr->GetRelativePos();
		mAttribute.position = float4(position.x, position.y, position.z, 1.0f);
		mAttribute.direction = float4(tr->Forward().x, tr->Forward().y, tr->Forward().z, 0.0f);

		RenderMgr::PushLightAttribute(mAttribute);
	}


	void Com_Light3D::Render()
	{
		if (nullptr == mLightMaterial)
		{
			return;
		}

		//Com_Transform* tr = GetOwner()->GetComponent<Com_Transform>();
		//tr->BindGPU();

		ConstBuffer* cb = RenderMgr::GetConstBuffer(eCBType::numberOfLight);

		tCB_NumberOfLight data = {};
		data.numberOfLight = (uint)RenderMgr::GetLights().size();
		data.indexOfLight = mIndex;

		cb->SetData(&data);
		cb->BindData(eShaderStageFlag::VS | eShaderStageFlag::PS);

		mVolumeMesh->BindBuffer();
		mLightMaterial->Bind();
		mVolumeMesh->Render();
	}


	void Com_Light3D::SetType(eLightType type)
	{
		mAttribute.lightType = (int)type;
		if (mAttribute.lightType == (int)eLightType::Directional)
		{
			mVolumeMesh = ResMgr::Find<Mesh>(strKey::Default::mesh::RectMesh);
			mLightMaterial = ResMgr::Find<Material>(strKey::Default::material::LightDirMaterial);
		}
		else if (mAttribute.lightType == (int)eLightType::Point)
		{
			mVolumeMesh = ResMgr::Find<Mesh>(strKey::Default::mesh::SphereMesh);
			mLightMaterial = ResMgr::Find<Material>(strKey::Default::material::LightPointMaterial);
		}
		else if (mAttribute.lightType == (int)eLightType::Spot)
		{
			ERROR_MESSAGE_W(L"미구현");
		}
	}
	
}
