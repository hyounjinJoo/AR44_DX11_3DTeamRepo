#include "PCH_Engine.h"
#include "guiFBXConverter.h"
#include "Application.h"
#include "PathMgr.h"
#include "define_Util.h"
#include <commdlg.h>

namespace gui
{
	guiFBXConverter::guiFBXConverter()
		: guiWindow("FBX Converter")
	{
	}

	guiFBXConverter::~guiFBXConverter()
	{
	}

	void guiFBXConverter::Init()
	{



	}


	void guiFBXConverter::Update()
	{


	}


	void guiFBXConverter::UpdateUI()
	{
		if (ImGui::Button("Convert FBX", ImVec2(100.f, 40.f)))
		{
			//풀경로를 받아올 주소 변수를 만들어주고
			std::basic_string<TCHAR> stringPath;

			//프로그램 주소를 넣어놓는다.
			stringPath = std::fs::current_path().string<TCHAR>();
			stringPath.reserve(MAX_PATH);

			//파일 열기 창에 전달할 설정 구조체를 설정해준다.
			OPENFILENAME OpenFile = {};

			OpenFile.lStructSize = sizeof(OPENFILENAME);	//구조체 크기
			OpenFile.hwndOwner = mh::Application::GetHwnd();	//관리 핸들
			OpenFile.lpstrFilter = TEXT("*.fbx File\0*.fbx\0모든 파일\0*.*"); 	//파일 형식
			OpenFile.lpstrFile = stringPath.data();	//경로가 저장될 변수 주소
			OpenFile.nMaxFile = MAX_PATH;	//최대 경로 자릿수


			//만들어진 풀 경로를 FullPath에 보낸다.
			if (GetOpenFileName(&OpenFile))
			{
				std::fs::path filePath = stringPath;
				std::string upperExt = filePath.extension().string();
				StringConv::UpperCase(upperExt);
				if (".FBX" == upperExt)
				{
					int a = 3;
				}
			}
		
		}


	}
}

