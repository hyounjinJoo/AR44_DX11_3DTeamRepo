#pragma once
#include "Scene.h"

namespace mh
{
	class SceneManager
	{
	public:
		static void Initialize();
		static void Update();
		static void FixedUpdate();
		static void Render();
		static void Destroy();
		static void Release();

		static void		LoadScene(enums::eSceneType _Type);
		static Scene*	GetActiveScene() { return mActiveScene; }

	private:
		static std::vector<Scene*>	mScenes;
		static Scene*				mActiveScene;
	};
}
