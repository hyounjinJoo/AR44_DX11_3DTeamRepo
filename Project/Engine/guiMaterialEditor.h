#pragma once
#include "guiWindow.h"
#include "guiComboBox.h"

//Material 전방선언
namespace mh
{
	class Material;
}

namespace gui
{
	class guiMaterialEditor
		: public guiWindow
	{
	public:
		guiMaterialEditor();
		virtual ~guiMaterialEditor();

		virtual void Init() override;
		virtual void Update() override;
		virtual void UpdateUI() override;

	private:
		void RefreshShaderSettingFiles();
		void UpdateShader();
		void UpdateTextureList();
		void UpdateRenderingMode();
		void UpdateMtrlConstBuffer();

	private:
		std::shared_ptr<mh::Material>	mTargetMaterial;

		//기존의 재질을 수정중인지 신규 재질을 수정 중인지
		bool							mbNewMaterial;

		guiComboBox		mShaderCombo;

		guiComboBox		mCurrentMaterialsCombo;

		guiComboBox		mRenderingModeCombo;
	};
}


