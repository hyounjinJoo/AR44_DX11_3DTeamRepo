#pragma once
#include "yaComponent.h"
#include "yaGraphics.h"


namespace ya
{
	class Light : public Component
	{
	public:
		Light();
		~Light();

		virtual void Initalize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		graphics::LightAttribute GetLightAttribute() { return mAttribute; }

		void SetDiffuse(Vector4 diffuse) { mAttribute.diffuse = diffuse; }
		void SetAmbient(Vector4 ambient) { mAttribute.ambient = ambient; }
		void SetType(eLightType type) { mAttribute.type = type; }
		void SetRadius(float radius) { mAttribute.radius = radius; }
		void SetAngle(float angle) { mAttribute.angle = angle; }
		Vector4 GetDiffuse() { mAttribute.diffuse; }
		eLightType GetType() { return (eLightType)mAttribute.type; }
		float GetRadius() { mAttribute.radius; }
		float GetAngle() { mAttribute.angle; }

	private:
		graphics::LightAttribute mAttribute;
	};
}
