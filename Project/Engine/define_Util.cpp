#include "PCH_Engine.h"
#include "define_Util.h"
#include "Application.h"

void StringConv::ConvertUTF8ToUnicode(__in const std::string_view _src, __out std::wstring& _dest)
{
	_dest.clear();

	int srcsize = (int)_src.size();
	int len = ::MultiByteToWideChar(CP_UTF8, 0, _src.data(), srcsize, nullptr, 0);

	_dest.resize(len);
	::MultiByteToWideChar(CP_UTF8, 0, _src.data(), srcsize, _dest.data(), (int)_dest.size());
}

void StringConv::ConvertUnicodeToUTF8(__in const std::wstring_view _src, __out std::string& _dest)
{
	_dest.clear();

	int srcsize = (int)_src.size();
	int len = ::WideCharToMultiByte(CP_UTF8, 0, _src.data(), srcsize, nullptr, 0, nullptr, nullptr);

	_dest.resize(len);
	::WideCharToMultiByte(CP_UTF8, 0, _src.data(), srcsize, _dest.data(), (int)_dest.size(), nullptr, nullptr);
}

std::wstring StringConv::ConvertUTF8ToUnicode(const std::string_view _src)
{
	std::wstring result;

	int srcsize = (int)_src.size();
	int len = ::MultiByteToWideChar(CP_ACP, 0, _src.data(), srcsize, nullptr, 0);

	result.resize(len);
	::MultiByteToWideChar(CP_ACP, 0, _src.data(), srcsize, result.data(), (int)result.size());

	return result;
}


std::string StringConv::ConvertUnicodeToUTF8(const std::wstring_view _src)
{
	std::string result;

	int srcsize = (int)_src.size();
	int len = ::WideCharToMultiByte(CP_UTF8, 0, _src.data(), srcsize, nullptr, 0, nullptr, nullptr);

	result.resize(len);

	::WideCharToMultiByte(CP_UTF8, 0, _src.data(), srcsize, result.data(), (int)result.size(), nullptr, nullptr);

	return result;
}


std::filesystem::path mh::WinAPI::FileDialog(const std::filesystem::path& _baseDirectory, const std::filesystem::path& _extension)
{
	std::vector<std::filesystem::path> extensions = { _extension };
	return FileDialog(_baseDirectory, extensions);
}

std::filesystem::path mh::WinAPI::FileDialog(const std::filesystem::path& _baseDirectory, const std::vector<std::filesystem::path>& _extensions)
{
	//풀경로를 받아올 주소 변수를 만들어주고
	std::basic_string<TCHAR> stringPath;

	//프로그램 주소를 넣어놓는다.
	stringPath = _baseDirectory.string<TCHAR>();
	stringPath += TEXT("\\file");
	stringPath.resize(MAX_PATH);

	//파일 열기 창에 전달할 설정 구조체를 설정해준다.
	OPENFILENAME OpenFile = {};

	OpenFile.lStructSize = sizeof(OPENFILENAME);	//구조체 크기
	OpenFile.hwndOwner = mh::Application::GetHwnd();	//관리 핸들

	auto addStrEnd = [](std::vector<TCHAR>& tStr)->void
		{
#ifdef UNICODE
			tStr.push_back(L'\0');
#else
			tStr.push_back('\0');
#endif
		};

	std::vector<TCHAR> extensionFilters;
	size_t cursorPos{};
	for (size_t i = 0; i < _extensions.size(); ++i)
	{
		//해당 확장자의 이름 설정
		const std::basic_string<TCHAR>& ext = _extensions[i].string<TCHAR>();
		std::copy(ext.begin(), ext.end(), std::back_inserter(extensionFilters));
		std::basic_string<TCHAR> fileName = TEXT(" File");
		std::copy(fileName.begin(), fileName.end(), std::back_inserter(extensionFilters));

		//확장자 포맷 설정
		addStrEnd(extensionFilters);
#ifdef UNICODE
		extensionFilters.push_back(L'*');
#else
		extensionFilters.push_back('*');
#endif
		
		std::copy(ext.begin(), ext.end(), std::back_inserter(extensionFilters));
		addStrEnd(extensionFilters);
	}
	std::basic_string<TCHAR> allFiles = TEXT("All Files");
	std::copy(allFiles.begin(), allFiles.end(), std::back_inserter(extensionFilters));
	addStrEnd(extensionFilters);
	allFiles = TEXT("*.*");
	std::copy(allFiles.begin(), allFiles.end(), std::back_inserter(extensionFilters));
	addStrEnd(extensionFilters);
	addStrEnd(extensionFilters);

	OpenFile.lpstrFilter = extensionFilters.data();
	OpenFile.lpstrFile = stringPath.data();	//경로가 저장될 변수 주소
	OpenFile.nMaxFile = MAX_PATH;	//최대 경로 자릿수

	//이 플래그를 넣어주지 않으면 std::current_path가 불러온 주소로 바껴버림
	OpenFile.Flags = OFN_NOCHANGEDIR;

	//만들어진 풀 경로를 FullPath에 보낸다.
	if (FALSE == GetOpenFileName(&OpenFile))
	{
		stringPath.clear();
	}

	return stringPath;
}
