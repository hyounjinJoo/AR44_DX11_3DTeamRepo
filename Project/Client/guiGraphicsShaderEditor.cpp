#include "PCH_Client.h"

#include "guiGraphicsShaderEditor.h"

#include <Engine/PathMgr.h>
#include <Engine/GraphicsShader.h>
#include <Engine/json-cpp/json.h>


constexpr const char* DXGI_FORMAT_String[] =
{
	"DXGI_FORMAT_UNKNOWN",
	"DXGI_FORMAT_R32G32B32A32_TYPELESS",
	"DXGI_FORMAT_R32G32B32A32_FLOAT",
	"DXGI_FORMAT_R32G32B32A32_UINT",
	"DXGI_FORMAT_R32G32B32A32_SINT",
	"DXGI_FORMAT_R32G32B32_TYPELESS",
	"DXGI_FORMAT_R32G32B32_FLOAT",
	"DXGI_FORMAT_R32G32B32_UINT",
	"DXGI_FORMAT_R32G32B32_SINT",
	"DXGI_FORMAT_R16G16B16A16_TYPELES",
	"DXGI_FORMAT_R16G16B16A16_FLOAT",
	"DXGI_FORMAT_R16G16B16A16_UNORM",
	"DXGI_FORMAT_R16G16B16A16_UINT",
	"DXGI_FORMAT_R16G16B16A16_SNORM",
	"DXGI_FORMAT_R16G16B16A16_SINT",
	"DXGI_FORMAT_R32G32_TYPELESS",
	"DXGI_FORMAT_R32G32_FLOAT",
	"DXGI_FORMAT_R32G32_UINT",
	"DXGI_FORMAT_R32G32_SINT",
	"DXGI_FORMAT_R32G8X24_TYPELESS",
	"DXGI_FORMAT_D32_FLOAT_S8X24_UINT",
	"DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS",
	"DXGI_FORMAT_X32_TYPELESS_G8X24_UINT",
	"DXGI_FORMAT_R10G10B10A2_TYPELESS",
	"DXGI_FORMAT_R10G10B10A2_UNORM",
	"DXGI_FORMAT_R10G10B10A2_UINT",
	"DXGI_FORMAT_R11G11B10_FLOAT",
	"DXGI_FORMAT_R8G8B8A8_TYPELESS",
	"DXGI_FORMAT_R8G8B8A8_UNORM",
	"DXGI_FORMAT_R8G8B8A8_UNORM_SRGB",
	"DXGI_FORMAT_R8G8B8A8_UINT",
	"DXGI_FORMAT_R8G8B8A8_SNORM",
	"DXGI_FORMAT_R8G8B8A8_SINT",
	"DXGI_FORMAT_R16G16_TYPELESS",
	"DXGI_FORMAT_R16G16_FLOAT",
	"DXGI_FORMAT_R16G16_UNORM",
	"DXGI_FORMAT_R16G16_UINT",
	"DXGI_FORMAT_R16G16_SNORM",
	"DXGI_FORMAT_R16G16_SINT",
	"DXGI_FORMAT_R32_TYPELESS",
	"DXGI_FORMAT_D32_FLOAT",
	"DXGI_FORMAT_R32_FLOAT",
	"DXGI_FORMAT_R32_UINT",
	"DXGI_FORMAT_R32_SINT",
	"DXGI_FORMAT_R24G8_TYPELESS",
	"DXGI_FORMAT_D24_UNORM_S8_UINT",
	"DXGI_FORMAT_R24_UNORM_X8_TYPELESS",
	"DXGI_FORMAT_X24_TYPELESS_G8_UINT",
	"DXGI_FORMAT_R8G8_TYPELESS",
	"DXGI_FORMAT_R8G8_UNORM",
	"DXGI_FORMAT_R8G8_UINT",
	"DXGI_FORMAT_R8G8_SNORM",
	"DXGI_FORMAT_R8G8_SINT",
	"DXGI_FORMAT_R16_TYPELESS",
	"DXGI_FORMAT_R16_FLOAT",
	"DXGI_FORMAT_D16_UNORM",
	"DXGI_FORMAT_R16_UNORM",
	"DXGI_FORMAT_R16_UINT",
	"DXGI_FORMAT_R16_SNORM",
	"DXGI_FORMAT_R16_SINT",
	"DXGI_FORMAT_R8_TYPELESS",
	"DXGI_FORMAT_R8_UNORM",
	"DXGI_FORMAT_R8_UINT",
	"DXGI_FORMAT_R8_SNORM",
	"DXGI_FORMAT_R8_SINT",
	"DXGI_FORMAT_A8_UNORM",
	"DXGI_FORMAT_R1_UNORM",
	"DXGI_FORMAT_R9G9B9E5_SHAREDEXP",
	"DXGI_FORMAT_R8G8_B8G8_UNORM",
	"DXGI_FORMAT_G8R8_G8B8_UNORM",
	"DXGI_FORMAT_BC1_TYPELESS",
	"DXGI_FORMAT_BC1_UNORM",
	"DXGI_FORMAT_BC1_UNORM_SRGB",
	"DXGI_FORMAT_BC2_TYPELESS",
	"DXGI_FORMAT_BC2_UNORM",
	"DXGI_FORMAT_BC2_UNORM_SRGB",
	"DXGI_FORMAT_BC3_TYPELESS",
	"DXGI_FORMAT_BC3_UNORM",
	"DXGI_FORMAT_BC3_UNORM_SRGB",
	"DXGI_FORMAT_BC4_TYPELESS",
	"DXGI_FORMAT_BC4_UNORM",
	"DXGI_FORMAT_BC4_SNORM",
	"DXGI_FORMAT_BC5_TYPELESS",
	"DXGI_FORMAT_BC5_UNORM",
	"DXGI_FORMAT_BC5_SNORM",
	"DXGI_FORMAT_B5G6R5_UNORM",
	"DXGI_FORMAT_B5G5R5A1_UNORM",
	"DXGI_FORMAT_B8G8R8A8_UNORM",
	"DXGI_FORMAT_B8G8R8X8_UNORM",
	"DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM",
	"DXGI_FORMAT_B8G8R8A8_TYPELESS",
	"DXGI_FORMAT_B8G8R8A8_UNORM_SRGB",
	"DXGI_FORMAT_B8G8R8X8_TYPELESS",
	"DXGI_FORMAT_B8G8R8X8_UNORM_SRGB",
	"DXGI_FORMAT_BC6H_TYPELESS",
	"DXGI_FORMAT_BC6H_UF16",
	"DXGI_FORMAT_BC6H_SF16",
	"DXGI_FORMAT_BC7_TYPELESS",
	"DXGI_FORMAT_BC7_UNORM",
	"DXGI_FORMAT_BC7_UNORM_SRGB",
	"DXGI_FORMAT_AYUV",
	"DXGI_FORMAT_Y410",
	"DXGI_FORMAT_Y416",
	"DXGI_FORMAT_NV12",
	"DXGI_FORMAT_P010",
	"DXGI_FORMAT_P016",
	"DXGI_FORMAT_420_OPAQUE",
	"DXGI_FORMAT_YUY2",
	"DXGI_FORMAT_Y210",
	"DXGI_FORMAT_Y216",
	"DXGI_FORMAT_NV11",
	"DXGI_FORMAT_AI44",
	"DXGI_FORMAT_IA44",
	"DXGI_FORMAT_P8",
	"DXGI_FORMAT_A8P8",
	"DXGI_FORMAT_B4G4R4A4_UNORM",
	"DXGI_FORMAT_P208",
	"DXGI_FORMAT_V208",
	"DXGI_FORMAT_V408",
	"DXGI_FORMAT_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE",
	"DXGI_FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE",
	"DXGI_FORMAT_FORCE_UINT",
};
constexpr const size_t DXGI_FORMAT_String_Size = sizeof(DXGI_FORMAT_String) / sizeof(const char*);

namespace gui
{
	namespace stdfs = std::filesystem;

	GraphicsShaderEditor::GraphicsShaderEditor()
		: Widget("Graphics Shader Editor")
		, mSemanticNames()
		, mEditTarget()
	{
		mEditTarget = std::make_unique<mh::GraphicsShader>();
		mSemanticNames.resize(1);
	}
	GraphicsShaderEditor::~GraphicsShaderEditor()
	{
	}


	//TODO: 여기 코드 정리하기
	void GraphicsShaderEditor::Update()
	{
		static int SemanticEditTarget = -1;
		static bool ShowModalEditorPopup = false;
		
		constexpr const char* cEdit = "Edit##";
		constexpr const char* cDelete = "Delete##";

		for (size_t i = 0; i < mInputElements.size(); ++i)
		{
			constexpr const char* cSemantic = "Semantic ";
			std::string SemanticNameIdx = cSemantic;
			SemanticNameIdx += std::to_string(i);
			
			if (mInputElements[i].SemanticName)
			{
				SemanticNameIdx += ": ";
				SemanticNameIdx += mInputElements[i].SemanticName;
			}
				
			ImGui::Text(SemanticNameIdx.c_str());
			ImGui::SameLine(100.f);

			std::string Edit = cEdit;
			Edit += std::to_string(i);
			if (ImGui::Button(Edit.c_str()))
			{
				SemanticEditTarget = (int)i;
				ShowModalEditorPopup = true;
			}

			ImGui::SameLine();

			std::string Delete = cDelete;
			Delete += std::to_string(i);
			if (ImGui::Button(Delete.c_str()))
			{
				auto SemanticNameIter = mSemanticNames.begin() + i;
				auto mInputElementsIter = mInputElements.begin() + i;


				//다음 이터레이터를 받아서 순회 시작
				SemanticNameIter = mSemanticNames.erase(SemanticNameIter);
				mInputElementsIter = mInputElements.erase(mInputElementsIter);

				while (mInputElementsIter != mInputElements.end())
				{
					//1개씩 빼줌
					mInputElementsIter->SemanticIndex -= 1u;

					++mInputElementsIter;
				}




				break;
			}

			if (ShowModalEditorPopup)
			{
				ImGui::SetNextWindowSize(ImVec2{ 400.f, 500.f });
				ImGui::OpenPopup("Edit Layout Element");
				if (ImGui::BeginPopupModal("Edit Layout Element", &ShowModalEditorPopup))
				{
					static D3D11_INPUT_ELEMENT_DESC newdesc = {};

					constexpr float AlignWidth = 100.f;

					ImGui::Text("SemanticName");
					ImGui::SameLine(AlignWidth);
					if (ImGui::InputText("SemanticName##", &mSemanticNames[SemanticEditTarget]))
					{
						newdesc.SemanticName = mSemanticNames[SemanticEditTarget].c_str();
					}

					ImGui::Text("DXGI_FORMAT");
					ImGui::SameLine(AlignWidth);
					static int CurSelected = -1;
					const char* Preview = "";
					if (0 <= CurSelected && CurSelected < DXGI_FORMAT_String_Size)
						Preview = DXGI_FORMAT_String[CurSelected];
					if (ImGui::BeginCombo("DXGI_FORMAT##", Preview))
					{
						
						for (int n = 0; n < (int)DXGI_FORMAT_String_Size; ++n)
						{
							bool isSelected = (CurSelected == n);

							//여기는 콤보박스에서 특정 값이 선택(클릭)되었을 떄 진입한다.
							if (true == ImGui::Selectable(DXGI_FORMAT_String[n], isSelected))
							{
								CurSelected = n;
								if ((DXGI_FORMAT)n <= DXGI_FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE)
									newdesc.Format = (DXGI_FORMAT)n;
								else
									newdesc.Format = DXGI_FORMAT_FORCE_UINT;

							}

							if (isSelected)
							{
								ImGui::SetItemDefaultFocus();
							}
						}

						ImGui::EndCombo();
					}


					ImGui::Text("AlignedByteOffset");
					ImGui::SameLine(AlignWidth);

					static int byteOffset = 0;
					if (ImGui::InputInt("AlignedByteOffset##", &byteOffset) && byteOffset >= 0)
					{
						newdesc.AlignedByteOffset = (UINT)byteOffset;
					}

					//LPCSTR SemanticName; -> 사용
					//UINT SemanticIndex; -> 미사용(같은 시멘틱 일름으로 여러 개를 만들 경우 사용)
					//DXGI_FORMAT Format; -> 사용
					//UINT InputSlot; -> 미사용
					//UINT AlignedByteOffset; -> 사용
					//D3D11_INPUT_CLASSIFICATION InputSlotClass; -> 인스턴싱에 사용
					//UINT InstanceDataStepRate; -> 미사용

					const static ImVec2 buttonSize(100, 30);
					ImGui::SetCursorPosY(ImGui::GetWindowHeight() - buttonSize.y - 10.f);
					ImGui::Separator();
					ImGui::SetCursorPosX(ImGui::GetWindowWidth() - buttonSize.x * 2.f - 30.f);


					if (ImGui::Button("OK", buttonSize))
					{
						mInputElements[SemanticEditTarget] = newdesc;

						SemanticEditTarget = -1;
						ShowModalEditorPopup = false;
					}
					ImGui::SameLine();
					if (ImGui::Button("Cancel", buttonSize))
					{
						SemanticEditTarget = -1;
						ShowModalEditorPopup = false;
					}


					//ImGui::SameLine();
					//if (ImGui::Button("Apply", buttonSize))
					//{

					//	ShowModalEditorPopup = false;
					//}

					ImGui::EndPopup();
				}
			}
		}

		if(ImGui::Button("Add Input Layout Desc"))
		{
			D3D11_INPUT_ELEMENT_DESC desc{};
			desc.SemanticIndex = (UINT)mInputElements.size();
			mInputElements.emplace_back(desc);
			mSemanticNames.emplace_back(std::string());
		}

		static int 



		ImGui::Spacing();
		ImGui::Separator();
		ImGui::PushItemWidth(100.f);
		if (ImGui::Button("Find Default Shaders"))
		{
			CreateDefaultShaders();
		}
	}



	bool GraphicsShaderEditor::CreateDefaultShaders()
	{
		struct tShaderGroup
		{
			stdfs::path FileName[(int)mh::define::eGSStage::END];
		};
		std::unordered_map<stdfs::path, tShaderGroup> umapGSGroup;


		//쉐이더 바이트코드 경로 확인
		stdfs::path shaderCSODir = stdfs::current_path();
		shaderCSODir /= mh::define::strKey::DirName_ShaderBin;
		if (false == stdfs::exists(shaderCSODir))
		{
			stdfs::create_directories(shaderCSODir);
			MessageBoxW(nullptr, L"쉐이더 파일이 없습니다.", nullptr, MB_OK);
			return false;
		}


		//쉐이더 바이트코드 경로 순회
		for (stdfs::directory_iterator iter(shaderCSODir); iter != stdfs::end(iter); ++iter)
		{
			if (iter->is_directory())
				continue;

			std::string fileName = iter->path().filename().string();
			
			for (int i = 0; i < (int)mh::define::eGSStage::END; ++i)
			{
				size_t pos = fileName.find(mh::define::strKey::ArrGSPrefix[i]);
				if (std::string::npos == pos)
					continue;

				std::string baseFileName = fileName;
				baseFileName.erase(pos, std::strlen(mh::define::strKey::ArrGSPrefix[i]));

				umapGSGroup[baseFileName].FileName[i] = fileName;
			}

		}

		//쉐이더 세팅 파일 경로 확인
		std::vector<std::string> vecNewShaderGroup;
		stdfs::path ShaderSettingDir = mh::PathMgr::GetRelResourcePath(eResourceType::GraphicsShader);
		if (false == stdfs::exists(ShaderSettingDir))
		{
			stdfs::create_directories(ShaderSettingDir);
		}
		
		//map을 순회 돌아주면서
		for (const auto& iter : umapGSGroup)
		{
			stdfs::path ShaderFilePath = ShaderSettingDir / iter.first.filename().replace_extension(mh::define::strKey::Ext_ShaderSetting);

			//파일이 존재하지 않으면 json 파일 초기화 및 생성을 해준다.
			if (false == stdfs::exists(ShaderFilePath))
			{
				std::ofstream ofs(ShaderFilePath);
				if (false == ofs.is_open())
				{
					ERROR_MESSAGE_W(L"파일 생성에 실패했습니다.");
					return false;
				}

				Json::Value jVal;
				{
					mh::GraphicsShader DummyGS;
					DummyGS.SetKey(iter.first.string());
					if (mh::eResultFail(DummyGS.SaveJson(&jVal)))
					{
						ERROR_MESSAGE_W(L"json 파일 저장에 실패했습니다.");
						return false;
					}
				}

				for (int i = 0; i < (int)mh::define::eGSStage::END; ++i)
				{
					jVal[mh::define::strKey::ArrGSPrefix[i]] = iter.second.FileName[i].string();
				}

				ofs << jVal;
				ofs.close();

				vecNewShaderGroup.push_back(iter.first.string());
			}
		}

		//새 쉐이더 그룹이 발견되면 어떤 쉐이더 그룹이 새로 만들었는지 
		if (false == vecNewShaderGroup.empty())
		{
			std::string Message = "New Shader Detected!\n";
			for (size_t i = 0; i < vecNewShaderGroup.size(); ++i)
			{
				Message += vecNewShaderGroup[i];
				Message += "\n";
			}

			MessageBoxA(nullptr, Message.c_str(), nullptr, MB_OK);
		}

		return true;
	}


	
}
