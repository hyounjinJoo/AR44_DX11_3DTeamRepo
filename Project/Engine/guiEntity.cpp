#include "PCH_Engine.h"
#include "guiEntity.h"
#include "json-cpp/json.h"

namespace gui
{
	UINT32 guiEntity::gIDNext{};

	guiEntity::guiEntity(const std::string_view _strName)
		: mID(++gIDNext)
		, mName(_strName)
		, mStrKey(_strName)
		, mbSaveEnable()	
	{
	}


	guiEntity::~guiEntity()
	{
	}
	
}


