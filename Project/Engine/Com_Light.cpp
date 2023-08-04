#include "PCH_Engine.h"

#include "Com_Light.h"
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
	Com_Light::Com_Light()
		: IComponent(eComponentType::Light)
		, mVolumeMesh()
		, mLightMaterial()
		, mIndex()
	{
		RenderMgr::AddLight(this);

	}

	Com_Light::Com_Light(const Com_Light& _other)
		:IComponent(eComponentType::Light)
		, mIndex(_other.mIndex)
		, mAttribute(_other.mAttribute)
		, mVolumeMesh(_other.mVolumeMesh)
		, mLightMaterial(_other.mLightMaterial)
	{
		RenderMgr::AddLight(this);
	}

	Com_Light::~Com_Light()
	{
		RenderMgr::RemoveLight(this);
	}

	eResult Com_Light::SaveJson(Json::Value* _pJVal)
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

		Json::MHSaveValue(_pJVal, JSON_KEY_PAIR(mAttribute));

		return eResult::Success;
	}

	eResult Com_Light::LoadJson(const Json::Value* _pJVal)
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
	
		Json::MHLoadValue(_pJVal, JSON_KEY_PAIR(mAttribute));

		SetType(mAttribute.type);

		//불러오기 실패 시 기본값으로 적용
		if (false == Json::MHLoadValue(_pJVal, JSON_KEY_PAIR(mAttribute.type)))
		{
			mAttribute.type = eLightType::Directional;
		}

		return eResult::Success;
	}

	void Com_Light::Init()
	{

	}

	void Com_Light::Update()
	{

	}

	void Com_Light::FixedUpdate()
	{
		Com_Transform& tr = GetOwner()->GetTransform();

		if (eLightType::Point == mAttribute.type)
		{
			tr.SetScale(float3(mAttribute.radius * 5.f, mAttribute.radius * 5.f, mAttribute.radius * 5.f));
		}

		float3 position = tr.GetPosition();
		mAttribute.position = float4(position.x, position.y, position.z, 1.0f);
		mAttribute.direction = float4(tr.Foward().x, tr.Foward().y, tr.Foward().z, 0.0f);

		RenderMgr::PushLightAttribute(mAttribute);
	}

	void Com_Light::Render()
	{
		if (nullptr == mLightMaterial)
		{
			return;
		}

		Com_Transform* tr = GetOwner()->GetComponent<Com_Transform>();
		tr->SetConstBuffer();

		ConstBuffer* cb = RenderMgr::GetConstBuffer(eCBType::Light);

		LightCB data = {};
		data.NumberOfLight = (uint)RenderMgr::GetLights().size();
		data.IndexOfLight = mIndex;

		cb->SetData(&data);
		cb->BindData(eShaderStageFlag::VS | eShaderStageFlag::PS);

		mVolumeMesh->BindBuffer();
		mLightMaterial->Bind();
		mVolumeMesh->Render();
	}


	void Com_Light::SetType(eLightType type)
	{
		mAttribute.type = type;
		if (mAttribute.type == eLightType::Directional)
		{
			mVolumeMesh = ResMgr::Find<Mesh>(strKey::Default::mesh::RectMesh);
			mLightMaterial = ResMgr::Find<Material>(strKey::Default::material::LightDirMaterial);
		}
		else if (mAttribute.type == eLightType::Point)
		{
			mVolumeMesh = ResMgr::Find<Mesh>(strKey::Default::mesh::SphereMesh);
			mLightMaterial = ResMgr::Find<Material>(strKey::Default::material::LightPointMaterial);
		}
		else if (mAttribute.type == eLightType::Spot)
		{
			ERROR_MESSAGE_W(L"미구현");
		}
	}
	
}
