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
		LoadProjMeshDataCombo();
	}


	void guiFBXConverter::UpdateUI()
	{
		if (CheckThread())
			return;


		HilightText("FBX Source File Path");
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

		HilightText("Convert Setting");
		ImGui::InputText("Output Directory Name", &mOutputDirName);

		ImGui::Checkbox("Static Mesh?", &mbStatic);

		ConvertFBXButton();

		ImGui::Dummy(ImVec2(0.0f, 30.0f));
		ImGui::Separator();

		AddAnimationFromSameModeling();
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
			else if (std::future_status::ready == status)
			{
				if (mh::eResultSuccess(mFutureData.get()))
				{
					NOTIFICATION_W(L"FBX 변환 성공.");
				}
				else
				{
					NOTIFICATION_W(L"FBX 변환 실패.");
				}
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

	void guiFBXConverter::AddAnimationFromSameModeling()
	{
		HilightText("Add Animation to Project MeshData");
		ImGui::Dummy(ImVec2(0.f, 10.f));

		mProjMeshDataCombo.FixedUpdate();
		if (ImGui::Button("Refresh List"))
		{
			LoadProjMeshDataCombo();
		}


		ImGui::Dummy(ImVec2(0.f, 20.f));
		if(ImGui::Button("Add Animation", ImVec2(0.f, 35.f)))
		{
			if (mFBXPath.empty())
			{
				NOTIFICATION_W(L"FBX 경로를 설정하지 않았습니다.");
			}
			else if (mProjMeshDataCombo.GetCurrentSelected().strName.empty())
			{
				NOTIFICATION_W(L"목표 메쉬 데이터를 설정하지 않았습니다.");
			}
			else
			{
				mh::MeshData::AddAnimationFromFBX(mFBXPath, mProjMeshDataCombo.GetCurrentSelected().strName);
			}
		}
	}

	void guiFBXConverter::MultiThreadedFBXLoad()
	{
		std::mutex mtx{};
		std::unique_lock<std::mutex> lock(mtx);

		std::shared_ptr<mh::MeshData> meshData = std::make_shared<mh::MeshData>();
		mh::eResult result = meshData->ConvertFBX(mFBXPath, mbStatic, mOutputDirName);

		if (mh::eResultFail(result))
		{
			MessageBoxW(nullptr, L"FBX 로드 실패.", nullptr, MB_OK);
		}

		mPromise.set_value(result);

		mThreadWorking = false;
	}

	void guiFBXConverter::LoadProjMeshDataCombo()
	{
		mProjMeshDataCombo.SetKey("MeshData List");
		const std::fs::path& meshPath = mh::PathMgr::GetContentPathRelative(mh::define::eResourceType::MeshData);
		for (const auto& entry : std::fs::directory_iterator(meshPath))
		{
			if (std::fs::is_directory(entry.path()))
			{
				mProjMeshDataCombo.AddItem(entry.path().filename().string());
			}
		}
	}
}

