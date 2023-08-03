#include "EnginePCH.h"

#include "Com_Light.h"
#include "Com_Transform.h"
#include "GameObject.h"
#include "RenderMgr.h"
#include "ResMgr.h"
#include "Mesh.h"
#include "Material.h"

#include "ConstBuffer.h"

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

	Com_Light::~Com_Light()
	{
		RenderMgr::RemoveLight(this);
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
			tr.SetScale(Vector3(mAttribute.radius * 5.f, mAttribute.radius * 5.f, mAttribute.radius * 5.f));
		}

		Vector3 position = tr.GetPosition();
		mAttribute.position = Vector4(position.x, position.y, position.z, 1.0f);
		mAttribute.direction = Vector4(tr.Foward().x, tr.Foward().y, tr.Foward().z, 0.0f);

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
		data.NumberOfLight = (UINT)RenderMgr::GetLights().size();
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
