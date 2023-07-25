#pragma once
#include "IComponent.h"
#include "define_GPU.h"


namespace mh
{
	using namespace mh::math;
	using namespace mh::define;
	class Light : public IComponent
	{
	public:
		Light();
		~Light();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		GPU::tLightAttribute GetLightAttribute() { return mAttribute; }

		void SetDiffuse(const Vector4& diffuse) { mAttribute.diffuse = diffuse; }
		void SetSpecular(const Vector4& spec) { mAttribute.specular = spec; }
		void SetAmbient(const Vector4& ambient) { mAttribute.ambient = ambient; }
		void SetType(eLightType type) { mAttribute.type = type; }
		void SetRadius(float radius) { mAttribute.radius = radius; }
		void SetAngle(float angle) { mAttribute.angle = angle; }
		const Vector4& GetDiffuse() { mAttribute.diffuse; }
		eLightType GetType() { return (eLightType)mAttribute.type; }
		float GetRadius() { mAttribute.radius; }
		float GetAngle() { mAttribute.angle; }

	private:
		GPU::tLightAttribute mAttribute;
	};
}
