#pragma once
#include "guiWindow.h"
#include "guiComboBox.h"
#include <thread>
#include <future>


namespace mh
{
	class MeshData;
}


namespace gui
{

	class guiFBXConverter :
		public guiWindow
	{
	public:
		guiFBXConverter();
		virtual ~guiFBXConverter();

		virtual void Init() override;
		virtual void UpdateUI() override;

	private:
		bool CheckThread();
		void ChooseFBXButton();
		void ConvertFBXButton();
		void AddAnimationFromSameModeling();

		void MultiThreadedFBXLoad();


		void LoadProjMeshDataCombo();

	private:
		std::string		mFBXPath;
		std::string		mOutputDirName;
		bool			mbStatic;

		std::jthread	mLoaderThread;
		std::promise<mh::eResult>	mPromise;
		std::future<mh::eResult>	mFutureData;
		bool			mThreadWorking;

		guiComboBox		mProjMeshDataCombo;
	};
}


