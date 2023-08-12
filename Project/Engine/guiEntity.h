#pragma once

#include "imgui.h"
#include "imgui_stdlib.h"
#include <string>

namespace gui
{
	class guiEntity
	{
	public:
		guiEntity(const std::string_view _strName);
		virtual ~guiEntity();

		UINT32 GetID() const { return mID; }

		const std::string& GetName() const { return mName; }

		void MakeUniqueKeyByName() { mStrKey += "##"; mStrKey += std::to_string(mID); }
		void SetKey(const std::string_view _strKey) { mStrKey = _strKey; }
		const std::string& GetKey() const { return mStrKey; }
		

	private:
		static UINT32				gIDNext;
		const UINT32				mID;

		//이름(중복 가능)
		std::string					mName;
		
		//Key(중복 불가)
		std::string					mStrKey;
	};
}


