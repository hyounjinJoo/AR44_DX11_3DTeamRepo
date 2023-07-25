#include "EnginePCH.h"

#include "Com_Light.h"
#include "Transform.h"
#include "GameObject.h"
#include "Renderer.h"

namespace mh
{
	Com_Light::Com_Light()
		: IComponent(eComponentType::Light)
	{

	}

	Com_Light::~Com_Light()
	{

	}

	void Com_Light::Initialize()
	{

	}

	void Com_Light::Update()
	{

	}

	void Com_Light::FixedUpdate()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 position = tr->GetPosition();
		mAttribute.position = Vector4(position.x, position.y, position.z, 1.0f);
		mAttribute.direction = Vector4(tr->Foward().x, tr->Foward().y, tr->Foward().z, 0.0f);
		//mAttribute.type = define::eLightType::Directional;

		renderer::PushLightAttribute(mAttribute);
	}

	void Com_Light::Render()
	{

	}
}