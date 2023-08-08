#pragma once

namespace mh
{
	class ContentsClassInitializer
	{
	public:
		static void Init();

	private:
		static void InitCom();
		static void InitScript();
		static void InitScene();
		static void InitContents();
	};
}

