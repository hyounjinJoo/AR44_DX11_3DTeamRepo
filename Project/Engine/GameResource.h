#pragma once
#include "Entity.h"
#include "Enums.h"

namespace mh
{
	using namespace mh::enums;

	class GameResource : public Entity
	{
	public:
		GameResource(eResourceType type);
		virtual ~GameResource();

		virtual HRESULT Load(const std::filesystem::path& path) = 0;

		const std::wstring& GetKey() { return mKey; }
		const std::wstring& GetPath() { return mPath; }
		eResourceType GetType() { return mType; }

		void SetKey(const std::wstring& key) { mKey = key; }
		void SetPath(const std::wstring& path) { mPath = path; }

	private:
		std::wstring mKey;
		std::wstring mPath;
		eResourceType mType;
	};
}
