#include "PCH_Client.h"
#include "guiEntity.h"

namespace gui
{
	UINT32 guiEntity::gIDNext{};

	guiEntity::guiEntity(const std::string_view _strKey)
		: mID(++gIDNext)
		, mStrKey(_strKey)
	{
	}




	guiEntity::~guiEntity()
	{
	}
}


