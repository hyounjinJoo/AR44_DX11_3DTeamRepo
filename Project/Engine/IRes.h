#pragma once
#include "Entity.h"
#include "define_Res.h"

namespace mh
{
	using namespace mh::define;

	class IRes 
		: public Entity
	{
	public:
		IRes(eResourceType type);
		virtual ~IRes();

		virtual HRESULT Load(const std::filesystem::path& path) = 0;

		eResourceType GetResType() { return mType; }

		void SetEngineDefaultRes(bool _bIsDefault) { mbEngineDefaultRes = _bIsDefault; }
		bool IsEngineDefaultRes() const { return mbEngineDefaultRes; }

	private:
		eResourceType mType;

		bool mbEngineDefaultRes;
	};
}
