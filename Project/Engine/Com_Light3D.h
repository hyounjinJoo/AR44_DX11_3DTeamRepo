#pragma once
#include "ILight.h"
#include "define_GPU.h"
#include "defaultShader/SH_Func_Light.hlsli"

namespace mh
{
	class Mesh;
	class Material;
	class Com_Light3D : public ILight
	{
	public:
		Com_Light3D();

		Com_Light3D(const Com_Light3D& _other);
		CLONE(Com_Light3D);

		virtual ~Com_Light3D();

		virtual eResult SaveJson(Json::Value* _pJVal) override;
		virtual eResult LoadJson(const Json::Value* _pJVal) override;

		//virtual void Init() override;
		//virtual void Update() override;
		virtual void FixedUpdate() override;

		//Camera가 호출
		virtual void Render() override;

		const tLightAttribute& GetLightAttribute() { return mAttribute; }
		
		void SetDiffuse(const float4& diffuse) { mAttribute.color.diffuse = diffuse; }
		void SetSpecular(const float4& spec) { mAttribute.color.specular = spec; }
		void SetAmbient(const float4& ambient) { mAttribute.color.ambient = ambient; }
		void SetLightType(define::eLightType type);
		void SetRadius(float radius) { mAttribute.radius = radius; }
		void SetAngle(float angle) { mAttribute.angle = angle; }
		const float4& GetDiffuse() { mAttribute.color.diffuse; }
		define::eLightType GetType() { return (define::eLightType)mAttribute.lightType; }
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
