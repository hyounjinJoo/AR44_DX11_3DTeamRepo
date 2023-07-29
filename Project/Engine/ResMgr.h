#pragma once

#include "Singleton.h"

#include "define_Res.h"
#include "define_Struct.h"
#include "IRes.h"


#include "Mesh.h"
#include "MeshData.h"

#include "Texture.h"
#include "Material.h"

#include "AudioClip.h"
#include "Prefab.h"

#include "GraphicsShader.h"
#include "ComputeShader.h"

#include "IScript.h"


namespace mh
{
	using namespace mh::define;
	using namespace mh;
	namespace stdfs = std::filesystem;

	class ResMgr
	{
		friend class Application;
	public:
		template <typename T>
		static eResourceType GetResType();

		template <typename T>
		static std::shared_ptr<T> Load(const std::filesystem::path& _fileName, const std::string_view _strKey = "");

		template <typename T>
		static std::shared_ptr<T> Find(const std::string_view _strKey);

		static std::shared_ptr<IRes> Find(eResourceType _ResType, const std::string_view _strKey);

		template <typename T>
		static const std::unordered_map<std::string, std::shared_ptr<IRes>, tUmap_StringViewHasher, std::equal_to<>>&
			GetResources();

		static void Add(const std::string_view _strKey, std::shared_ptr<IRes> _Res);
		
	private:
		static void Init();
		static void Release();
	
	private:
		static std::unordered_map<std::string, std::shared_ptr<IRes>, tUmap_StringViewHasher, std::equal_to<>> mArrRes[(int)eResourceType::End];
	};


	template<typename T>
	inline eResourceType ResMgr::GetResType()
	{
		static_assert(std::is_base_of_v<IRes, T>);

		if constexpr (std::is_base_of_v<Mesh, T>)
		{
			return eResourceType::Mesh;
		}
		else if constexpr (std::is_base_of_v<MeshData, T>)
		{
			return eResourceType::MeshData;
		}
		else if constexpr (std::is_base_of_v<Texture, T>)
		{
			return eResourceType::Texture;
		}
		else if constexpr (std::is_base_of_v<Material, T>)
		{
			return eResourceType::Material;
		}
		else if constexpr (std::is_base_of_v<AudioClip, T>)
		{
			return eResourceType::AudioClip;
		}
		else if constexpr (std::is_base_of_v<Prefab, T>)
		{
			return eResourceType::Prefab;
		}
		else if constexpr (std::is_base_of_v<GraphicsShader, T>)
		{
			return eResourceType::GraphicsShader;
		}
		else if constexpr (std::is_base_of_v<ComputeShader, T>)
		{
			return eResourceType::ComputeShader;
		}
		
		return eResourceType::End;
	}

	template<typename T>
	inline std::shared_ptr<T> ResMgr::Load(const std::filesystem::path& _fileName, const std::string_view _strKey)
	{
		//IRes를 상속받는 클래스가 아닐 경우 컴파일 중지
		static_assert(std::is_base_of<IRes, T>::value);

		std::string strKey(_strKey);

		//Key가 비어있을 경우 파일명을 키값으로 사용한다.
		if (strKey.empty())
			strKey = _fileName.string();

		std::shared_ptr<T> pRes = Find<T>(strKey);

		// 이미 해당 키로 리소스가 있다면, 반환
		if (nullptr != pRes)
			return pRes;

		pRes = std::make_shared<T>();

		if (FAILED(pRes->Load(_fileName)))
			return nullptr;

		Add(strKey, pRes);

		return pRes;
	}

	template<typename T>
	inline std::shared_ptr<T> ResMgr::Find(const std::string_view _strKey)
	{
		eResourceType ResType = GetResType<T>();

		if (eResourceType::UNKNOWN == ResType)
			return nullptr;

		const auto& iter = mArrRes[(int)ResType].find(_strKey);
		
		if (iter == mArrRes[(int)ResType].end())
			return nullptr;

		return std::static_pointer_cast<T, IRes>(iter->second);
	}

	template<typename T>
	inline const std::unordered_map<std::string, std::shared_ptr<IRes>, tUmap_StringViewHasher, std::equal_to<>>& ResMgr::GetResources()
	{
		eResourceType Type = GetResType<T>();
		assert(eResourceType::UNKNOWN != Type);

		return mArrRes[(int)Type];
	}



	inline std::shared_ptr<IRes> ResMgr::Find(eResourceType _ResType, const std::string_view _strKey)
	{
		if (eResourceType::UNKNOWN == _ResType)
			return nullptr;

		const auto& iter = mArrRes[(int)_ResType].find(_strKey);

		if (iter == mArrRes[(int)_ResType].end())
			return nullptr;

		return iter->second;
	}

	inline void ResMgr::Add(const std::string_view _strKey, std::shared_ptr<IRes> _Res)
	{
		eResourceType ResType = _Res->GetResType();

		assert(nullptr == Find(ResType, _strKey));

		mArrRes[(int)ResType].insert(std::make_pair(std::string(_strKey), _Res));
	}



}
