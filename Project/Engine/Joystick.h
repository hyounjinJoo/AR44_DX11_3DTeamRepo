#pragma once


#include <cstdint>
#include <roapi.h>
#include <wrl.h>
#include <concrt.h>
#include <Xinput.h>
#include "windows.gaming.input.h"
#include "SimpleMath.h"

#pragma comment(lib, "runtimeobject.lib")

namespace mh
{
	enum class eVibrationPower
	{
		Zero,
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
		void Update();
		void CheckDeadZoneCase();
		void ProcessingVibration();
		ABI::Windows::Gaming::Input::GamepadReading GetState();

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
