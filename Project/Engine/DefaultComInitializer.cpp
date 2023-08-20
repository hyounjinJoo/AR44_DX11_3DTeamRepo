#include "PCH_Engine.h"
#include "DefaultComInitializer.h"

#include "ComMgr.h"
#include "define_Component.h"

#include "Com_Animator2D.h"
#include "Com_Animator3D.h"
#include "Com_AudioListener.h"
#include "Com_AudioSource.h"
#include "Com_Camera.h"
#include "Com_Light3D.h"
#include "Com_Renderer_Mesh.h"
#include "Com_Renderer_ParticleSystem.h"
#include "Com_Renderer_Sprite.h"
#include "Com_Transform.h"

#include "Script_Player.h"
#include "Script_Camera.h"

namespace mh
{
	using namespace mh::define;
	void DefaultComInitializer::Init()
	{
		ComMgr::AddComConstructor<Com_Animator2D>(strKey::Default::com::Com_Animator2D);
		ComMgr::AddComConstructor<Com_Animator3D>(strKey::Default::com::Com_Animator3D);
		ComMgr::AddComConstructor<Com_AudioListener>(strKey::Default::com::Com_AudioListener);
		ComMgr::AddComConstructor<Com_AudioSource>(strKey::Default::com::Com_AudioSource);
		ComMgr::AddComConstructor<Com_Camera>(strKey::Default::com::Com_Camera);
		ComMgr::AddComConstructor<Com_Light3D>(strKey::Default::com::Com_Light3D);
		ComMgr::AddComConstructor<Com_Renderer_Mesh>(strKey::Default::com::Com_Renderer_Mesh);
		ComMgr::AddComConstructor<Com_Renderer_ParticleSystem>(strKey::Default::com::Com_Renderer_ParticleSystem);
		ComMgr::AddComConstructor<Com_Renderer_Sprite>(strKey::Default::com::Com_Renderer_Sprite);
		ComMgr::AddComConstructor<Com_Transform>(strKey::Default::com::Com_Transform);

		ComMgr::AddComConstructor<Script_Player>(strKey::Default::com::Script_Player);
		ComMgr::AddComConstructor<Script_Camera>(strKey::Default::com::Script_Camera);
	}
}

