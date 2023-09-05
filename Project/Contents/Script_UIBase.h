#pragma once
#include <Engine/IScript.h>
#include <Engine/define_GPU.h>

//HLSL 헤더 파일을 그대로 include
#include <HLSL/UIBasic.hlsli>



namespace mh
{
	enum class eUIType
	{
		Basic,
		Gauge,
		Button
	};

	class Texture;
	class Com_Renderer_UIBase;
	class Material;
	class Script_UIBase :
		public IScript
	{
	public:
		Script_UIBase(eUIType _mode);
		Script_UIBase();

		virtual ~Script_UIBase();

		virtual void Init() override;
		void SetCustomMaterial(std::shared_ptr<Material> _material);
		void SetTexture(define::eTextureSlot _slot, std::shared_ptr<Texture> _tex);

	protected:
		Com_Renderer_UIBase* mUIComponent;

	private:
		void CreateUI();

		eUIType mUIMode;
	};
}


