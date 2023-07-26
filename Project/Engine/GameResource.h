#pragma once
#include "Entity.h"
#include "Enums.h"

namespace mh
{
	using namespace mh::enums;

	class GameResource 
		: public Entity
	{
	public:
		GameResource(eResourceType type);
		virtual ~GameResource();

		virtual HRESULT Load(const std::filesystem::path& path) = 0;

		const std::string& GetKey() { return mKey; }
		const std::string& GetPath() { return mPath; }
		eResourceType GetResType() { return mType; }

		void SetKey(const std::string& key) { mKey = key; }
		void SetPath(const std::string& path) { mPath = path; }

		void SetEngineDefaultRes(bool _bIsDefault) { mbEngineDefaultRes = _bIsDefault; }
		bool IsEngineDefaultRes() const { return mbEngineDefaultRes; }

	private:
		std::string mKey;
		std::string mPath;
		eResourceType mType;

		bool mbEngineDefaultRes;
	};
}
