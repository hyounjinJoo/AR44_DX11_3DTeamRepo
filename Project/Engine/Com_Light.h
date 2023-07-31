#pragma once
#include "IComponent.h"
#include "define_GPU.h"


namespace mh
{
	using namespace mh::math;
	using namespace mh::define;

	class Mesh;
	class Material;
	class Com_Light : public IComponent
	{
	public:
		Com_Light();
		virtual ~Com_Light();

		virtual void Init() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		const tLightAttribute& GetLightAttribute() { return mAttribute; }

		void SetDiffuse(const Vector4& diffuse) { mAttribute.diffuse = diffuse; }
		void SetSpecular(const Vector4& spec) { mAttribute.specular = spec; }
		void SetAmbient(const Vector4& ambient) { mAttribute.ambient = ambient; }
		void SetType(eLightType type);
		void SetRadius(float radius) { mAttribute.radius = radius; }
		void SetAngle(float angle) { mAttribute.angle = angle; }
		const Vector4& GetDiffuse() { mAttribute.diffuse; }
		eLightType GetType() { return (eLightType)mAttribute.type; }
		float GetRadius() { mAttribute.radius; }
		float GetAngle() { mAttribute.angle; }
		void SetIndex(UINT _idx) { mIndex = _idx; }

	private:
		tLightAttribute mAttribute;
		std::shared_ptr<Mesh> mVolumeMesh; // 광원의 범위를 표현하는 메쉬
		std::shared_ptr<Material> mLightMaterial;
		UINT mIndex;
	};

}
