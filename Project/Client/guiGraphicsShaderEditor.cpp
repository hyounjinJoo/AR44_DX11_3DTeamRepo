#include "PCH_Client.h"

#include "guiGraphicsShaderEditor.h"

#include <Engine/PathMgr.h>
#include <Engine/GraphicsShader.h>
#include <Engine/json-cpp/json.h>


namespace gui
{
	namespace stdfs = std::filesystem;

	GraphicsShaderEditor::GraphicsShaderEditor()
		: Widget("Graphics Shader Editor")
	{
	}
	GraphicsShaderEditor::~GraphicsShaderEditor()
	{
	}

	void GraphicsShaderEditor::Update()
	{
		if (ImGui::Button("Find Default Shaders"))
		{
			CreateDefaultShaders();
		}
	}


	void GraphicsShaderEditor::Close()
	{
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
