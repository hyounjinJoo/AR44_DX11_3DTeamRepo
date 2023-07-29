#pragma once

namespace mh
{
	class TimeMgr
	{
		friend class Application;
	public:
		static __forceinline float DeltaTime() { return mDeltaTime; }

		static void Update();
		static void Render(HDC _hdc);

	private:
		static void Init();
		static void Release();

	private:
		static float	mDeltaTime;

		static LARGE_INTEGER	mCpuFrequency;
		static LARGE_INTEGER    mPrevFrequency;
		static LARGE_INTEGER	mCurFrequency;
		static float			mOneSecond;
	};
}


