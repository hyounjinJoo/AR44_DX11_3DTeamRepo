#include "PCH_Engine.h"
#include "guiMaterialEditor.h"

#include "Material.h"
#include "PathMgr.h"
#include "ResMgr.h"
#include "GraphicsShader.h"
#include "Application.h"
#include "define_Util.h"


namespace gui
{
	STRKEY eRenderingMode_String[(int)mh::define::eRenderingMode::END] =
	{
		//Deffered
		"DefferdOpaque",
		"DefferdMask",

		"Light",

		//Forward
		"Opaque",
		"CutOut",
		"Transparent",
		"PostProcess",
	};



	guiMaterialEditor::guiMaterialEditor()
		: guiWindow("Material Editor")
		, mTargetMaterial()
		, mShaderCombo{}
		, mbNewMaterial()
	{
		mTargetMaterial = std::make_shared<mh::Material>();
	}
	guiMaterialEditor::~guiMaterialEditor()
	{
	}

	void guiMaterialEditor::Init()
	{
		//쉐이더 파일 목록 업데이트
		mShaderCombo.SetKey("Shader Lists");
		RefreshShaderSettingFiles();

		//렌더링 모드 UI 생성
		mRenderingModeCombo.SetKey("Rendering Mode");
		{
			for (int i = 0; i < (int)mh::define::eRenderingMode::END; ++i)
			{
				mRenderingModeCombo.AddItem(eRenderingMode_String[i]);
			}
		}
	}

	void guiMaterialEditor::Update()
	{
		if (mShaderCombo.IsSelectionChanged())
		{
			std::string shaderName = mShaderCombo.GetCurrentSelected().strName;
			if (false == shaderName.empty())
			{
				std::shared_ptr<mh::GraphicsShader> gs = mh::ResMgr::Load<mh::GraphicsShader>(shaderName);
			}
		}
	}

	void guiMaterialEditor::UpdateUI()
	{
		UpdateShader();

		ImGui::Separator();

		UpdateRenderingMode();

		ImGui::Separator();

		UpdateTextureList();

		ImGui::Separator();

		UpdateMtrlConstBuffer();
	}
	void guiMaterialEditor::RefreshShaderSettingFiles()
	{
		const std::fs::path& shaderPath = 
			mh::PathMgr::GetContentPathRelative(mh::define::eResourceType::GraphicsShader);

		if (false == std::fs::exists(shaderPath))
		{
			std::fs::create_directories(shaderPath);
			return;
		}

		mShaderCombo.Reset();

		std::fs::directory_iterator dirIter(shaderPath);
		for (auto& entry : dirIter)
		{
			std::fs::path relPath = entry.path().lexically_relative(shaderPath);
			mShaderCombo.AddItem(relPath.string());
		}
	}
	void guiMaterialEditor::UpdateShader()
	{
		std::string strCurShader = "Current Shader: ";
		{
			std::shared_ptr<mh::GraphicsShader> curShader = mTargetMaterial->GetShader();
			if (curShader)
			{
				strCurShader += curShader->GetKey();
			}
			else
			{
				strCurShader += "None";
			}
		}
		ImGui::Text(strCurShader.c_str());


		mShaderCombo.FixedUpdate();
		if (ImGui::Button("Refresh Shaders"))
		{
			RefreshShaderSettingFiles();
		}

		if (mShaderCombo.IsSelectionChanged())
		{
			const std::string& shaderKey = mShaderCombo.GetCurrentSelected().strName;
			if (false == shaderKey.empty())
			{
				std::shared_ptr<mh::GraphicsShader> GS = mh::ResMgr::Load<mh::GraphicsShader>(shaderKey);
				mTargetMaterial->SetShader(GS);
			}
		}

		std::string shaderKey = "Shader: ";
		std::shared_ptr<mh::GraphicsShader> shader = mTargetMaterial->GetShader();
		if (shader)
		{
			shaderKey += shader->GetKey();
		}
		else
		{
			shaderKey += "None";
		}
		ImGui::Text(shaderKey.c_str());
	}
	void guiMaterialEditor::UpdateTextureList()
	{
		for (int i = 0; i < (int)mh::define::eTextureSlot::END; ++i)
		{
			constexpr const char* texText = "Texture_";
			std::string text = texText;
			text += std::to_string(i);
			text += ": ";
			ImGui::Text(text.c_str());

			ImGui::SameLine();

			auto Tex = mTargetMaterial->GetTexture((mh::define::eTextureSlot)i);
			if (Tex)
			{
				ImGui::Text(Tex->GetKey().c_str());
			}
			else
			{
				ImGui::Text("None");
			}

			ImGui::SameLine();

			if (ImGui::Button("Load"))
			{
				const std::fs::path& texPath = mh::PathMgr::GetContentPathAbsolute(mh::define::eResourceType::Texture);
				
				std::vector<std::fs::path> vecExt{};
				for (size_t i = 0; i < mh::define::strKey::Ext_Tex_Size; ++i)
				{
					vecExt.push_back(mh::define::strKey::Ext_Tex[i]);
				}
				std::fs::path receivedPath = mh::WindowFileDialog(texPath, vecExt);
				if (false == receivedPath.empty())
				{
					receivedPath = receivedPath.lexically_relative(texPath);
					std::shared_ptr<mh::Texture> tex = mh::ResMgr::Load<mh::Texture>(receivedPath);
					if (tex)
					{
						mTargetMaterial->SetTexture((mh::define::eTextureSlot)i, tex);
					}
					else
					{
						ERROR_MESSAGE_W(L"텍스처 찾기 실패");
					}
				}

			}
			ImGui::SameLine();
			if (ImGui::Button("Remove"))
			{
				mTargetMaterial->SetTexture((mh::define::eTextureSlot)i, nullptr);
			}

		}
	}
	void guiMaterialEditor::UpdateRenderingMode()
	{
		mh::define::eRenderingMode mode = mTargetMaterial->GetRenderingMode();
		mRenderingModeCombo.SetCurrentIndex((int)mode);
		mRenderingModeCombo.FixedUpdate();
		if (mRenderingModeCombo.IsSelectionChanged())
		{
			int idx = mRenderingModeCombo.GetCurrentIndex();
			if (0 <= idx)
			{
				mode = (mh::define::eRenderingMode)idx;
				mTargetMaterial->SetRenderingMode(mode);
			}
		}
	}
	void guiMaterialEditor::UpdateMtrlConstBuffer()
	{
		ImGui::DragFloat4("Diffuse Color", 
			reinterpret_cast<float*>(&(mTargetMaterial->mCB.Diff)));

		ImGui::DragFloat4("Specular Color",
			reinterpret_cast<float*>(&(mTargetMaterial->mCB.Spec)));

		ImGui::DragFloat4("Ambient Color",
			reinterpret_cast<float*>(&(mTargetMaterial->mCB.Amb)));

		ImGui::DragFloat4("Emissive Color",
			reinterpret_cast<float*>(&(mTargetMaterial->mCB.Emv)));
	}
}
