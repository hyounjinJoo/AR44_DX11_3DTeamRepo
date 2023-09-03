#pragma once
#include <filesystem>
namespace std
{
	namespace fs = filesystem;
}

#include "define_Res.h"


namespace mh
{
	using namespace mh::define;
	class PathMgr
	{
		friend class Application;
	public:
		static void Init();
		
	public:
		static const std::fs::path& GetResPathAbsolute() { return mAbsoluteResPath; }
		static const std::fs::path& GetResPathRelative() { return mRelativeResPath; }

		static inline std::fs::path GetContentPathAbsolute(eResourceType _eResType);

		static const std::fs::path& GetContentPathRelative(eResourceType _eResType) { return mRelativePathContent[(int)_eResType]; }

		static const std::fs::path& GetShaderCSOPath() { return mRelativePath_ShaderCSO; }

		static inline std::fs::path CreateFullPathToContent(const std::fs::path& _filePath, define::eResourceType _resType);

		//Res/Texture/FBX/MyTex -> FBX/MyTex
		static inline std::fs::path MakePathStrKey(const std::fs::path& _fullPath);


	private:
		static void Release();

	private:
		static std::filesystem::path mAbsoluteResPath;
		static std::filesystem::path mRelativeResPath;
		static std::filesystem::path mRelativePathContent[(int)eResourceType::END];
		
		static std::filesystem::path mRelativePath_ShaderCSO;
	};

	inline std::filesystem::path PathMgr::GetContentPathAbsolute(eResourceType _eResType)
	{
		return mAbsoluteResPath / define::strKey::ArrResName[(int)_eResType];
	}

	inline std::filesystem::path PathMgr::CreateFullPathToContent(const std::filesystem::path& _filePath, define::eResourceType _resType)
	{
		MH_ASSERT(define::eResourceType::UNKNOWN != _resType);

		std::filesystem::path fullPath = GetContentPathRelative(_resType);
		fullPath /= _filePath;

		//에러 방지를 위한 코드
		std::fs::path checkDir = fullPath.parent_path();
		if (false == std::fs::exists(checkDir))
		{
			if (false == std::fs::create_directories(checkDir))
			{
				ERROR_MESSAGE_W(L"폴더 생성 실패.");
				fullPath.clear();
			}
		}

		return fullPath;
	}
	inline std::fs::path PathMgr::MakePathStrKey(const std::fs::path& _fullPath)
	{
		std::fs::path strKeyPath{};
		std::fs::path newPath = _fullPath.lexically_relative(mAbsoluteResPath);

		//비어있거나, Res 바깥의 폴더(../)이면 안됨
		if (newPath.empty() || (*newPath.begin()) == "..")
		{
			ERROR_MESSAGE_W(L"리소스 파일은 반드시 Res 폴더의 안쪽에 있어야 합니다.");
		}
		else
		{
			for (auto iter = ++newPath.begin(); iter != newPath.end(); ++iter)
			{
				strKeyPath /= *iter;
			}
		}
		return strKeyPath;
	}
}

