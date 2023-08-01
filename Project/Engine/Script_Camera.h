#pragma once
#include "IScript.h"

namespace mh
{
	class Script_Camera : public IScript
	{
	public:
		Script_Camera();
		virtual ~Script_Camera();

		virtual void Init() override;
		virtual void Update() override;
		virtual void Render() override;

	private:

	};
}
