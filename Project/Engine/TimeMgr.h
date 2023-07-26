#pragma once
#include "EnginePCH.h"

namespace mh
{
	class TimeMgr
	{
	public:
		static __forceinline float DeltaTime() { return mDeltaTime; }

		static void Initialize();
		static void Update();
		static void Render(HDC _hdc);

	private:
		static LARGE_INTEGER	mCpuFrequency;
		static LARGE_INTEGER    mPrevFrequency;
		static LARGE_INTEGER	mCurFrequency;

		static float			mDeltaTime;
		static float			mOneSecond;
	};
}


