#include "EnginePCH.h"
#include "Joystick.h"
#include "TimeMgr.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//StackoverFlow answer
//Sunius Dec 23, 2016 at 6:50
//https://stackoverflow.com/questions/41286503/how-to-use-the-namespace-windows-gaming-input-in-c-without-using-uwp
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	클래스 작성자 : 주현진(JHJ)
//	작업 일자 : 23.08.06 ~
//	클래스 내용 : Joystick을 사용하기 위한 Gamepad 클래스를 wrapping 한 클래스 
//	제공 기능
//	1. Joystick의 키 입력 상태 제공
//	2. Joystick의 Thumb Stick, Trigger의 Deadzone 제어
//  3. Joystick의 Vibration 제어(패드에서 지원하면 Trigger Vibration 기능 지원)
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


namespace mh
{
	Joystick::Joystick(Microsoft::WRL::ComPtr<ABI::Windows::Gaming::Input::IGamepad>& _gamepad)
		: mGamePad(_gamepad)
		, mGamePadReading{0, }
		, mGamePadVibration{0, }
		, mDeadZone()
		, mLVibeTimer(0.f)
		, mRVibeTimer(0.f)
		, mLTVibeTimer(0.f)
		, mRTVibeTimer(0.f)
	{
	}

	Joystick::~Joystick()
	{
	}

	void Joystick::Update()
	{
		if (!mGamePad)
		{
			return;
		}

		HRESULT hr = mGamePad->GetCurrentReading(&mGamePadReading);
		assert(SUCCEEDED(hr));

		CheckDeadZoneCase();
		ProcessingVibration();
	}


	void Joystick::CheckDeadZoneCase()
	{
		// Check Left ThumbStick In DeadZone
		float stickX = static_cast<float>(mGamePadReading.LeftThumbstickX);   // returns a value between -1.0 and +1.0
		float stickY = static_cast<float>(mGamePadReading.LeftThumbstickY);   // returns a value between -1.0 and +1.0

		float deadzoneSquared = mDeadZone.LeftDeadZone * mDeadZone.LeftDeadZone;
		float xSquared = stickX * stickX;
		float ySquared = stickY * stickY;

		if ((xSquared + ySquared) > deadzoneSquared)
		{
			double ratio = 0.0;

			// X 축에 대한 DeadZone 처리
			if ((0.0 != (std::abs(stickX) - mDeadZone.LeftDeadZone)) && 0.0 != (1.0 - mDeadZone.LeftDeadZone))
			{
				ratio = (std::abs(stickX) - mDeadZone.LeftDeadZone) / (1.0 - mDeadZone.LeftDeadZone);
				mGamePadReading.LeftThumbstickX = ratio;
			}
			else
			{
				mGamePadReading.LeftThumbstickX = std::abs(stickX) - mDeadZone.LeftDeadZone;
			}

			// Y 축에 대한 DeadZone 처리
			if ((0.0 != (std::abs(stickY) - mDeadZone.LeftDeadZone)) && 0.0 != (1.0 - mDeadZone.LeftDeadZone))
			{
				ratio = (std::abs(stickY) - mDeadZone.LeftDeadZone) / (1.0 - mDeadZone.LeftDeadZone);
				mGamePadReading.LeftThumbstickY = ratio;
			}
			else
			{
				mGamePadReading.LeftThumbstickY = std::abs(stickY) - mDeadZone.LeftDeadZone;
			}


			if (stickX < 0.0)
			{
				mGamePadReading.LeftThumbstickX *= -1.0;
			}
			if (stickY < 0.0)
			{
				mGamePadReading.LeftThumbstickY *= -1.0;
			}
		}
		else
		{
			mGamePadReading.LeftThumbstickX = 0.0;
			mGamePadReading.LeftThumbstickY = 0.0;
		}

		// Check Right ThumbStick In DeadZone
		stickX = static_cast<float>(mGamePadReading.RightThumbstickX);
		stickY = static_cast<float>(mGamePadReading.RightThumbstickY);

		deadzoneSquared = mDeadZone.RightDeadZone * mDeadZone.RightDeadZone;

		xSquared = stickX * stickX;
		ySquared = stickY * stickY;

		if ((xSquared + ySquared) > deadzoneSquared)
		{
			double ratio = 0.0;

			// X 축에 대한 DeadZone 처리
			if ((0.0 != (std::abs(stickX) - mDeadZone.RightDeadZone)) && (0.0 != (1.0 - mDeadZone.RightDeadZone)) )
			{
				ratio = (std::abs(stickX) - mDeadZone.RightDeadZone) / (1.0 - mDeadZone.RightDeadZone);
				mGamePadReading.RightThumbstickX = ratio;
			}
			else
			{
				mGamePadReading.RightThumbstickX = std::abs(stickX) - mDeadZone.RightDeadZone;
			}

			// Y 축에 대한 DeadZone 처리
			if ((0.0 != (std::abs(stickY) - mDeadZone.RightDeadZone)) && (0.0 != (1.0 - mDeadZone.RightDeadZone)))
			{
				ratio = (std::abs(stickY) - mDeadZone.RightDeadZone) / (1.0 - mDeadZone.RightDeadZone);
				mGamePadReading.RightThumbstickY = ratio;
			}
			else
			{
				mGamePadReading.RightThumbstickY = std::abs(stickY) - mDeadZone.RightDeadZone;
			}


			if (stickX < 0.0)
			{
				mGamePadReading.RightThumbstickX *= -1.0;
			}
			if (stickY < 0.0)
			{
				mGamePadReading.RightThumbstickY *= -1.0;
			}
		}
		else
		{
			mGamePadReading.RightThumbstickX = 0.0;
			mGamePadReading.RightThumbstickY = 0.0;
		}

		// Check Left Trigger in DeadZone
		float triggerValue = static_cast<float>(mGamePadReading.LeftTrigger);

		if (triggerValue > mDeadZone.LeftTriggerDeadZone)
		{
			double ratio = 0.0;
			
			ratio = triggerValue - mDeadZone.LeftTriggerDeadZone / 1.0;
			mGamePadReading.LeftTrigger = std::fmin(1.0, std::fmax(0.0, ratio));
		}
		else
		{
			mGamePadReading.LeftTrigger = 0.0;
		}

		// Check Right Trigger in DeadZone
		triggerValue = static_cast<float>(mGamePadReading.RightTrigger);

		if (triggerValue > mDeadZone.RightTriggerDeadZone)
		{
			double ratio = 0.0;
			if (0.0 != triggerValue - mDeadZone.RightTriggerDeadZone)
			{
				ratio = triggerValue - mDeadZone.RightTriggerDeadZone / 1.0;
				mGamePadReading.RightTrigger = std::fmin(1.0, std::fmax(0.0, ratio));
			}
		}
		else
		{
			mGamePadReading.RightTrigger = 0.0;
		}
	}

	void Joystick::ProcessingVibration()
	{
		float time = TimeMgr::DeltaTime();
		
		mLVibeTimer -= time;
		mRVibeTimer -= time;
		mLTVibeTimer -= time;
		mRTVibeTimer -= time;

		if (0.f < mLVibeTimer)
		{
			mGamePadVibration.LeftMotor = mLVibePower;
		}
		if (0.f < mRVibeTimer)
		{
			mGamePadVibration.RightMotor = mRVibePower;
		}
		if (0.f < mLTVibeTimer)
		{
			mGamePadVibration.LeftTrigger = mLTVibeTimer;
		}
		if (0.f < mRTVibeTimer)
		{
			mGamePadVibration.RightTrigger = mRTVibeTimer;
		}

		mGamePad->put_Vibration(mGamePadVibration);
	}

	void Joystick::ClearVibration()
	{
		mLVibeTimer = 0.f;
		mRVibeTimer = 0.f;
		mLTVibeTimer = 0.f;
		mRTVibeTimer = 0.f;

		mLVibePower = 0.f;
		mRVibePower = 0.f;
		mLTVibePower = 0.f;
		mRTVibePower = 0.f;
	}

	void Joystick::SetVibration(eVibrationTarget _targetMotor, eVibrationPower _motorPower, eVibrationDuration _duration)
	{
		float durationTime = static_cast<int>(_duration) + 1.f;
		durationTime *= 0.5f;

		float power = static_cast<int>(_motorPower) * 0.25f;

		switch (_targetMotor)
		{
		case mh::eVibrationTarget::Left:
			mLVibeTimer = durationTime;
			mLVibePower = power;
			break;
		case mh::eVibrationTarget::Right:
			mRVibeTimer = durationTime;
			mRVibePower = power;
			break;
		case mh::eVibrationTarget::LeftTrigger:
			mLTVibeTimer = durationTime;
			mLTVibePower = power;
			break;
		case mh::eVibrationTarget::RightTrigger:
			mRTVibeTimer = durationTime;
			mRTVibePower = power;
			break;
		case mh::eVibrationTarget::Both:
			mLVibeTimer = durationTime;
			mLVibePower = power;
			mRVibeTimer = durationTime;
			mRVibePower = power;
			break;
		case mh::eVibrationTarget::BothTrigger:
			mLTVibeTimer = durationTime;
			mLTVibePower = power;
			mRTVibeTimer = durationTime;
			mRTVibePower = power;
			break;
		default:
			break;
		}
	}
}
