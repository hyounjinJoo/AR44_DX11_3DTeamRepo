#pragma once
#include "guiWindow.h"
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


		void MultiThreadedFBXLoad();

	private:
		std::string		mFBXPath;
		std::string		mOutputDirName;
		bool			mbStatic;

		std::jthread	mLoaderThread;
		std::promise<std::shared_ptr<mh::MeshData>>	mPromise;
		std::future<std::shared_ptr<mh::MeshData>>	mFutureData;
		bool			mThreadWorking;
	};
}


