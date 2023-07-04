#include "yaLight.h"
#include "yaTransform.h"
#include "yaGameObject.h"
#include "yaRenderer.h"

namespace ya
{
	Light::Light()
		: Component(eComponentType::Light)
	{

	}

	Light::~Light()
	{

	}

	void Light::Initalize()
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
		mAttribute.direction = Vector4(tr->Right().x, tr->Right().y, tr->Right().z, 1.0f);
		//mAttribute.type = enums::eLightType::Directional;

		renderer::PushLightAttribute(mAttribute);
	}

	void Light::Render()
	{

	}
}
