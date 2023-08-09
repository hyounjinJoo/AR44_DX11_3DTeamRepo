#pragma once


#include <cstdint>
#include <roapi.h>
#include <wrl.h>
#include <Xinput.h>
#include "windows.gaming.input.h"
#include "SimpleMath.h"

#pragma comment(lib, "runtimeobject.lib")

namespace mh
{
	// 왼, 오, 왼 트리거, 오른 트리거, 왼오, 왼오 트리거
	enum class eVibrationTarget
	{
		Left,
		Right,
		LeftTrigger,
		RightTrigger,
		Both,
		BothTrigger,
	};

	// 0.25 씩 증가
	enum class eVibrationPower
	{
		Zero,
		Little,
		Small,
		Middle,
		Max,
		End,
	};

	enum class eVibrationDuration
	{
		Sec0_5,
		Sec1_0,
		Sec1_5,
		Sec2_0,
		Sec2_5,
		Sec3_0,
		End,
	};

	struct tStickDeadZone
	{
		float LeftDeadZone;
		float RightDeadZone;
		float LeftTriggerDeadZone;
		float RightTriggerDeadZone;

		tStickDeadZone()
			: LeftDeadZone(0.01f)
			, RightDeadZone(0.01f)
			, LeftTriggerDeadZone(0.01f)
			, RightTriggerDeadZone(0.01f)
		{
		}
	};

	class Joystick
	{
	public:
		// construtor destructor
		Joystick(Microsoft::WRL::ComPtr<ABI::Windows::Gaming::Input::IGamepad>& gamepad);
		~Joystick();

		// delete Function
		Joystick() = delete;
		Joystick(const Joystick& _Other) = delete;
		Joystick(Joystick&& _Other) noexcept = delete;
		Joystick& operator=(const Joystick& _Other) = delete;
		Joystick& operator=(Joystick&& _Other) noexcept = delete;

	public:
		Microsoft::WRL::ComPtr<ABI::Windows::Gaming::Input::IGamepad> GetStick() { return mGamePad; }
		void Update();
		void CheckDeadZoneCase();
		void ProcessingVibration();
		ABI::Windows::Gaming::Input::GamepadReading GetState() { return mGamePadReading; }

		void ClearVibration();
		void SetVibration(eVibrationTarget targetMotor, eVibrationPower motorPower, eVibrationDuration duration);
		void SetDeadZone(float _leftThumb, float _rightThumb, float _leftTrigger, float _rightTrigger)
		{
			mDeadZone.LeftDeadZone = _leftThumb;
			mDeadZone.RightDeadZone = _rightThumb;
			mDeadZone.LeftTriggerDeadZone = _leftTrigger;
			mDeadZone.RightTriggerDeadZone = _rightTrigger;
		}

	private:
		Microsoft::WRL::ComPtr<ABI::Windows::Gaming::Input::IGamepad> mGamePad;
		ABI::Windows::Gaming::Input::GamepadReading mGamePadReading;
		ABI::Windows::Gaming::Input::GamepadVibration mGamePadVibration;

		tStickDeadZone mDeadZone;

		float mLVibeTimer;
		float mRVibeTimer;
		float mLTVibeTimer;
		float mRTVibeTimer;

		DOUBLE mLVibePower;
		DOUBLE mRVibePower;
		DOUBLE mLTVibePower;
		DOUBLE mRTVibePower;
	};
}
