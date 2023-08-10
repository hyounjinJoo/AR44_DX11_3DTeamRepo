#pragma once

#include "imgui.h"
#include "imgui_stdlib.h"

namespace gui
{
	class guiEntity
	{
	public:
		guiEntity(const std::string_view _strKey);
		virtual ~guiEntity();

		void SetKey(const std::string_view _strKey) { mStrKey = _strKey; }
		const std::string& GetKey() const { return mStrKey; }
		UINT32 GetID() const { return mID; }

	private:
		static UINT32				gIDNext;

		const UINT32				mID;
		std::string					mStrKey;
	};
}


