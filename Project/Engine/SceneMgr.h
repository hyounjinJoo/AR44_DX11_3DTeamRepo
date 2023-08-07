#pragma once
#include "IScene.h"

namespace mh
{
	class SceneMgr
	{
	public:
		static void Init();
		static void Update();
		static void FixedUpdate();
		static void Render();
		static void Destroy();
		static void Release();

		static void		LoadScene(const std::string_view _strKey);
		static IScene*	GetActiveScene() { return mActiveScene.get(); }

		template <typename T>
		static void AddSceneConstructor(const std::string_view _strKey);
		

	private:
		static std::unique_ptr<IScene>				mActiveScene;


		static std::unordered_map<std::string_view, std::function<IScene* ()>> mUmapSceneConstructor;
	};

	template<typename T>
	inline void SceneMgr::AddSceneConstructor(const std::string_view _strKey)
	{
		static_assert(std::is_base_of_v<IScene, T>);
		mUmapSceneConstructor.insert(std::make_pair(_strKey,
			[]()->T*
			{
				return new T;
			}
		));
	}
}
