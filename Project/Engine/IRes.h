#pragma once
#include "Entity.h"
#include "define_Res.h"

#include "json-cpp/json-forwards.h"

namespace mh
{
	using namespace mh::define;

	class IRes 
		: public Entity
	{
	public:
		IRes(eResourceType _type);
		virtual ~IRes();

		//일반적으로 리소스는 별도의 파일이 존재하고, 그것만 로드해주면 되기 때문에
		//Save를 할 일이 없음 -> 딱히 구현 안해도 될경우 재정의 안해도 됨
		//Graphics Shader, Compute Shader 같은 경우에만 Save() 함수를 구현해주면 될듯
		virtual eResult Save(const std::filesystem::path& _path) { return eResult::Success; }
		virtual eResult Load(const std::filesystem::path& _path) = 0;

		virtual eResult SaveJson(Json::Value* _pJVal) override;
		virtual eResult LoadJson(const Json::Value* _pJVal) override;

		eResourceType GetResType() { return mType; }

		void SetEngineDefaultRes(bool _bIsDefault) { mbEngineDefaultRes = _bIsDefault; }
		bool IsEngineDefaultRes() const { return mbEngineDefaultRes; }

	private:
		const eResourceType mType;
		bool mbEngineDefaultRes;
	};
}
