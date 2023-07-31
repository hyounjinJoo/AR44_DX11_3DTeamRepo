#include "EnginePCH.h"

#include "Com_Light.h"
#include "Com_Transform.h"
#include "GameObject.h"
#include "RenderMgr.h"
#include "ResMgr.h"
#include "Mesh.h"

namespace mh
{
	Com_Light::Com_Light()
		: IComponent(eComponentType::Light)
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
			tr.SetScale(Vector3(mAttribute.radius * 2.f, mAttribute.radius * 2.f, mAttribute.radius * 2.f));
		}

		Vector3 position = tr.GetPosition();
		mAttribute.position = Vector4(position.x, position.y, position.z, 1.0f);
		mAttribute.direction = Vector4(tr.Foward().x, tr.Foward().y, tr.Foward().z, 0.0f);

		RenderMgr::PushLightAttribute(mAttribute);
	}

	void Com_Light::Render()
	{
		std::shared_ptr<Material> material
			= ResMgr::Find<Material>(strKey::Default::material::LightMaterial);

		if (material == nullptr)
		{
			return;
		}

		mVolumeMesh->BindBuffer();
		material->Bind();
		mVolumeMesh->Render();
	}


	void Com_Light::SetType(eLightType type)
	{
		mAttribute.type = type;
		if (mAttribute.type == eLightType::Directional)
		{
			mVolumeMesh = ResMgr::Find<Mesh>(strKey::Default::mesh::RectMesh);
		}
		else if (mAttribute.type == eLightType::Point)
		{
			mVolumeMesh = ResMgr::Find<Mesh>(strKey::Default::mesh::CircleMesh);
		}
		else if (mAttribute.type == eLightType::Spot)
		{
			//
		}
	}
	
}
