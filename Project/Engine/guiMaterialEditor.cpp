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
		, mCurContext()
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

		ImGui::Separator();

		if(ImGui::Button("New Material", ImVec2(0.f, 35.f)))
		{
			NewMaterial();
		}

		ImGui::SameLine();

		UpdateSaveLoad();
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

			std::string ButtonName = "Load##";
			ButtonName += std::to_string(i);
			if (ImGui::Button(ButtonName.c_str()))
			{
				const std::fs::path& texPath = mh::PathMgr::GetContentPathAbsolute(mh::define::eResourceType::Texture);
				
				std::vector<std::fs::path> vecExt{};
				for (size_t i = 0; i < mh::define::strKey::Ext_Tex_Size; ++i)
				{
					vecExt.push_back(mh::define::strKey::Ext_Tex[i]);
				}
				std::fs::path receivedPath = mh::WinAPI::FileDialog(texPath, vecExt);
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

			ButtonName = "Remove##";
			ButtonName += std::to_string(i);
			if (ImGui::Button(ButtonName.c_str()))
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
	void guiMaterialEditor::UpdateSaveLoad()
	{
		if (ImGui::Button("Save to File", ImVec2(0.f, 35.f)))
		{
			if (CheckSavePossible())
			{
				mCurContext = eContext::SaveToFile;
				mSaveLoadFileName = mTargetMaterial->GetKey();
			}
		}
		SaveToFile();
		
		ImGui::SameLine();

		if (ImGui::Button("Load from File", ImVec2(0.f, 35.f)))
		{
			mCurContext = eContext::LoadFromFile;
			mSaveLoadFileName.clear();

			//ResMgr로부터 로드되어있는 재질 목록 싹 수집
			mCurrentLoadedMtrl.Reset();
			const auto& materials = mh::ResMgr::GetResources<mh::Material>();
			for (auto& mtrl : materials)
			{
				mCurrentLoadedMtrl.AddItem(mtrl.second->GetKey());
			}
		}
		LoadFromFile();
	}
	bool guiMaterialEditor::CheckSavePossible()
	{
		bool bPossible = true;
		//저장 조건 확인
		if (nullptr == mTargetMaterial->GetShader() || mTargetMaterial->GetShader()->GetKey().empty())
		{
			MessageBoxW(nullptr, L"쉐이더를 설정하지 않았습니다.\n쉐이더는 반드시 설정해야 합니다.", nullptr, MB_OK);
			bPossible = false;
		}
		else if (nullptr == mTargetMaterial->mTextures[0])
		{
			if (IDNO == MessageBoxW(nullptr, L"첫 번째 텍스처(Tex0)가 없습니다.\n(에러는 아님 나중에 코드로 할거면 괜찮음)\n저장할까요?", L"Notification", MB_YESNO))
			{
				bPossible = false;
			}
		}

		return bPossible;
	}
	void guiMaterialEditor::SaveToFile()
	{
		//저장 컨텍스트일 경우
		if (eContext::SaveToFile == mCurContext)
		{
			//모달 창 팝업
			ImGui::SetNextWindowSize(ImVec2{ 400.f, 500.f });
			ImGui::OpenPopup("Material Save");
			if (ImGui::BeginPopupModal("Material Save"))
			{
				ImGui::InputText("File Name", &mSaveLoadFileName);

				if (ImGui::Button("Save##Material", ImVec2(100.f, 35.f)))
				{
					if (mSaveLoadFileName.empty())
					{
						MessageBoxW(nullptr, L"파일명을 지정하세요.", nullptr, MB_OK);
					}
					else
					{
						//저장할 때는 Key값을 바꿔야 하기 때문에 Clone 해서 저장해야 한다.
						//기존 리소스를 그대로 Save하게 되면 Key 값이 변경되어 에러가 발생할 수 있음.
						mTargetMaterial = std::shared_ptr<mh::Material>(mTargetMaterial->Clone());
						mTargetMaterial->SetKey(mSaveLoadFileName);
						mTargetMaterial->Save(mSaveLoadFileName);
						mCurContext = eContext::None;
					}
				}

				ImGui::SameLine();

				if (ImGui::Button("Cancel##Material", ImVec2(100.f, 35.f)))
				{
					mCurContext = eContext::None;
				}
				ImGui::EndPopup();
			}
		}
	}
	void guiMaterialEditor::LoadFromFile()
	{
		if (eContext::LoadFromFile == mCurContext)
		{
			//모달 창 팝업
			ImGui::SetNextWindowSize(ImVec2{ 400.f, 500.f });
			ImGui::OpenPopup("Material Load");
			if (ImGui::BeginPopupModal("Material Load"))
			{
				mCurrentLoadedMtrl.FixedUpdate();

				if (ImGui::Button("Load##Material", ImVec2(100.f, 35.f)))
				{
					const std::string& mtrlKey = mCurrentLoadedMtrl.GetCurrentSelected().strName;
					if (mtrlKey.empty())
					{
						NOTIFICATION_W(L"파일을 선택하세요.");
					}
					else
					{
						std::shared_ptr<mh::Material> mtrl = mh::ResMgr::Find<mh::Material>(mtrlKey);

						//엔진 기본 Material일 경우에는 Clone
						if (mtrl->IsEngineDefaultRes())
						{
							mtrl = std::shared_ptr<mh::Material>(mtrl->Clone());
							mtrl->SetEngineDefaultRes(false);
						}

						mTargetMaterial = mtrl;
						if (nullptr == mTargetMaterial)
						{
							NOTIFICATION_W(L"제대로 로드되지 않았습니다.");
							mTargetMaterial = std::make_shared<mh::Material>();
						}


						mCurContext = eContext::None;
					}
				}

				ImGui::SameLine();

				if (ImGui::Button("Cancel##Material", ImVec2(100.f, 35.f)))
				{
					mCurContext = eContext::None;
				}

				ImGui::Separator();

				if (ImGui::Button("Load From File", ImVec2(0.f, 35.f)))
				{
					const std::fs::path& mtrlDir = mh::PathMgr::GetContentPathAbsolute(mh::define::eResourceType::Material);
					std::fs::path filePath = mh::WinAPI::FileDialog(mtrlDir, ".json");
					if (false == std::fs::exists(filePath))
					{
						NOTIFICATION_W(L"파일을 찾지 못했습니다.");
					}
					else
					{
						filePath = filePath.lexically_relative(mtrlDir);
						mTargetMaterial = std::make_shared<mh::Material>();
						if (mh::eResultFail(mTargetMaterial->Load(filePath)))
						{
							std::wstring errMsg = filePath.wstring();
							errMsg += L"\n불러오기에 실패했습니다.";
							NOTIFICATION_W(errMsg.c_str());
							mTargetMaterial = std::make_shared<mh::Material>();
						}

						//불러왔던 불러오지 못했던 모달 빠져나가줌
						mCurContext = eContext::None;
					}
				}

				ImGui::EndPopup();
			}
		}
	}
	void guiMaterialEditor::NewMaterial()
	{
		mTargetMaterial = std::make_shared<mh::Material>();
	}
}
