#pragma once
#include "Entity.h"
#include "define_Res.h"
#include "PathMgr.h"
#include "json-cpp/json-forwards.h"

namespace mh
{
	class IRes 
		: public Entity
	{
	public:
		IRes(define::eResourceType _type);

		IRes(const IRes& _other) = default;

		virtual ~IRes();

		//BasePath를 저장
		virtual eResult Save(const std::fs::path& _filePath);
		virtual eResult Load(const std::fs::path& _filePath);

		virtual eResult SaveJson(Json::Value* _pJVal) override;
		virtual eResult LoadJson(const Json::Value* _pJVal) override;

		define::eResourceType GetResType() { return mType; }

		void SetEngineDefaultRes(bool _bIsDefault) { mbEngineDefaultRes = _bIsDefault; }
		bool IsEngineDefaultRes() const { return mbEngineDefaultRes; }

	private:
		const define::eResourceType mType;
		bool mbEngineDefaultRes;
	};

}
