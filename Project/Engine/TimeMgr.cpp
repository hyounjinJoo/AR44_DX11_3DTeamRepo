#include "EnginePCH.h"
#include "TimeMgr.h"

#include "AtExit.h"
#include "Application.h"

extern mh::Application application;

namespace mh
{
    float	            TimeMgr::mDeltaTime{};
    LARGE_INTEGER	    TimeMgr::mCpuFrequency{};
    LARGE_INTEGER       TimeMgr::mPrevFrequency{};
    LARGE_INTEGER	    TimeMgr::mCurFrequency{};
    float			    TimeMgr::mOneSecond{};

    void TimeMgr::Init()
    {
        AtExit::AddFunc(Release);

        //CPU 의 초당 반복되는 주파수를 얻어온다.
        QueryPerformanceFrequency(&mCpuFrequency);

        //프로그램을 시작했을때의 CPU 클럭 수
        QueryPerformanceCounter(&mPrevFrequency);
    }


    void TimeMgr::Release()
    {
        mDeltaTime = {};
        mCpuFrequency = {};
        mPrevFrequency = {};
        mCurFrequency = {};
        mOneSecond = {};
    }


    void TimeMgr::Update()
    {
        QueryPerformanceCounter(&mCurFrequency);

        float differenceInFrequancy 
            = static_cast<float>((mCurFrequency.QuadPart - mPrevFrequency.QuadPart));

        mDeltaTime = differenceInFrequancy / static_cast<float>(mCpuFrequency.QuadPart);
        mPrevFrequency.QuadPart = mCurFrequency.QuadPart;
    }

    void TimeMgr::Render(HDC _hdc)
    {
        static int iCount = 0;
        ++iCount;


        // 1 초에 한번
        mOneSecond += mDeltaTime;
        if (1.0f < mOneSecond)
        {
            HWND hWnd = application.GetHwnd();

            wchar_t szFloat[50] = {};
            float FPS = 1.f / mDeltaTime;
            swprintf_s(szFloat, 50, L"DeltaTime : %d", iCount);
            int iLen = static_cast<int>(wcsnlen_s(szFloat, 50));
            //TextOut(_dc, 10, 10, szFloat, iLen);

            SetWindowText(hWnd, szFloat);

            // 누적시간, 카운트 초기화
            mOneSecond = 0.f;
            iCount = 0;
        }
    }
}
