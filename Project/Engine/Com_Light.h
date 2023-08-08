#pragma once
#include "IComponent.h"
#include "define_GPU.h"


namespace mh
{
	using namespace mh;
	using namespace mh::define;

	class Mesh;
	class Material;
	class Com_Light : public IComponent
	{
	public:
		Com_Light();

		Com_Light(const Com_Light& _other);
		CLONE(Com_Light);

		virtual ~Com_Light();

		virtual eResult SaveJson(Json::Value* _pJVal) override;
		virtual eResult LoadJson(const Json::Value* _pJVal) override;

		virtual void Init() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		const tLightAttribute& GetLightAttribute() { return mAttribute; }

		void SetDiffuse(const float4& diffuse) { mAttribute.diffuse = diffuse; }
		void SetSpecular(const float4& spec) { mAttribute.specular = spec; }
		void SetAmbient(const float4& ambient) { mAttribute.ambient = ambient; }
		void SetType(eLightType type);
		void SetRadius(float radius) { mAttribute.radius = radius; }
		void SetAngle(float angle) { mAttribute.angle = angle; }
		const float4& GetDiffuse() { mAttribute.diffuse; }
		eLightType GetType() { return (eLightType)mAttribute.type; }
		float GetRadius() { mAttribute.radius; }
		float GetAngle() { mAttribute.angle; }
		void SetIndex(uint _idx) { mIndex = _idx; }

	private:
		tLightAttribute mAttribute;
		std::shared_ptr<Mesh> mVolumeMesh; // 광원의 범위를 표현하는 메쉬
		std::shared_ptr<Material> mLightMaterial;
		uint mIndex;
	};

}
