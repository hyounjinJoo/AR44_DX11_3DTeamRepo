#include "PCH_Engine.h"
#include "guiFBXConverter.h"
#include "Application.h"
#include "PathMgr.h"
#include "define_Util.h"
#include "MeshData.h"
#include "TimeMgr.h"

namespace gui
{
	guiFBXConverter::guiFBXConverter()
		: guiWindow("FBX Converter")
		, mFBXPath()
		, mOutputDirName()
		, mbStatic(true)
		, mThreadWorking()
	{
	}

	guiFBXConverter::~guiFBXConverter()
	{
	}

	void guiFBXConverter::Init()
	{



	}




	void guiFBXConverter::UpdateUI()
	{
		if (CheckThread())
			return;


		ImGui::Text("[FBX File Path]");
		if (mFBXPath.empty())
		{
			ImGui::Text("Empty");
		}
		else
		{
			ImGui::Text(mFBXPath.c_str());
		}

		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		ChooseFBXButton();

		ImGui::SameLine();
		
		if (ImGui::Button("Clear Path", ImVec2(0.f, 35.f)))
		{
			mFBXPath.clear();
		}

		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::Separator();
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		ImGui::InputText("Output Directory Name", &mOutputDirName);

		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::Separator();
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		ImGui::Checkbox("Static Mesh?", &mbStatic);

		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::Separator();
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		ConvertFBXButton();
	}
	bool guiFBXConverter::CheckThread()
	{
		if (mThreadWorking)
		{
			std::condition_variable cv{};
			std::mutex mtx{};
			std::unique_lock<std::mutex> lock(mtx);

			std::future_status status = mFutureData.wait_for(std::chrono::milliseconds(10));

			static float waitDot{};
			static int waitDotCount{};
			waitDot += mh::TimeMgr::DeltaTime();
			if (1.f < waitDot)
			{
				waitDot = 0.f;
				++waitDotCount;
				if (4 <= waitDotCount)
				{
					waitDotCount = 0;
				}
			}
			if (std::future_status::timeout == status)
			{
				std::string loading = "Loading";
				for (int i = 0; i < waitDotCount; ++i)
				{
					loading += ".";
				}
				ImGui::Button(loading.c_str());
			}
		}

		return mThreadWorking;
	}
	void guiFBXConverter::ChooseFBXButton()
	{
		if (ImGui::Button("Choose FBX File", ImVec2(0.f, 35.f)))
		{
			std::vector<std::fs::path> extensions = { ".fbx" };

			std::fs::path filePath = mh::WinAPI::FileDialog(std::fs::current_path(), extensions);
			mFBXPath = filePath.string();

			mOutputDirName = filePath.stem().string();
		}
	}


	void guiFBXConverter::ConvertFBXButton()
	{
		if (ImGui::Button("Convert FBX File", ImVec2(0.f, 35.f)))
		{
			if (mFBXPath.empty())
			{
				MessageBoxW(nullptr, L"FBX 경로를 설정하지 않았습니다.", nullptr, MB_OK);
				return;
			}
			else if (mOutputDirName.empty())
			{
				MessageBoxW(nullptr, L"출력 폴더를 설정하지 않았습니다.", nullptr, MB_OK);
				return;
			}
			
			//promise 초기화
			mPromise = {};
			mFutureData = mPromise.get_future();
			mLoaderThread = std::jthread(&guiFBXConverter::MultiThreadedFBXLoad, this);
			mThreadWorking = true;
		}
	}

	void guiFBXConverter::MultiThreadedFBXLoad()
	{
		std::shared_ptr<mh::MeshData> meshData = std::make_shared<mh::MeshData>();
		mh::eResult result = meshData->LoadAndConvertFBX(mFBXPath, mbStatic, mOutputDirName);

		if (mh::eResultFail(result))
		{
			MessageBoxW(nullptr, L"FBX 로드 실패.", nullptr, MB_OK);
			mPromise.set_value(nullptr);
		}
		else
		{
			mPromise.set_value(meshData);
		}

		std::mutex mtx{};
		std::unique_lock<std::mutex> lock(mtx);
		mThreadWorking = false;
	}


}

