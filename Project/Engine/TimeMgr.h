#pragma once
#include "Singleton.h"

namespace mh
{
	class TimeMgr
		: public Singleton<TimeMgr>
	{
		SINGLETON(TimeMgr);
	public:
		static __forceinline float DeltaTime() { return mDeltaTime; }

		void Initialize();
		void Update();
		void Render(HDC _hdc);

	private:
		static float	mDeltaTime;

		LARGE_INTEGER	mCpuFrequency;
		LARGE_INTEGER    mPrevFrequency;
		LARGE_INTEGER	mCurFrequency;
		float			mOneSecond;
	};
}


