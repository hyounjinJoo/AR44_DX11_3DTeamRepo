#include "Light.h"
#include "Transform.h"
#include "GameObject.h"
#include "Renderer.h"

namespace mh
{
	Light::Light()
		: IComponent(eComponentType::Light)
	{

	}

	Light::~Light()
	{

	}

	void Light::Initialize()
	{

	}

	void Light::Update()
	{

	}

	void Light::FixedUpdate()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 position = tr->GetPosition();
		mAttribute.position = Vector4(position.x, position.y, position.z, 1.0f);
		mAttribute.direction = Vector4(tr->Foward().x, tr->Foward().y, tr->Foward().z, 0.0f);
		//mAttribute.type = enums::eLightType::Directional;

		renderer::PushLightAttribute(mAttribute);
	}

	void Light::Render()
	{

	}
}
