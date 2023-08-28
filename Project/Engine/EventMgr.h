#pragma once
#include "define_Enum.h"

namespace mh
{
    class Prefab;
    class GameObject;
    class EventMgr
    {
        friend class Application;
    public:
        enum class eEventType
        {
            SpawnGameObj,
            DeleteGameObj,
            AddChild,
            MoveGameObjLayer,
        };
        struct tEvent
        {
            eEventType Type;
            DWORD_PTR	wParam;
            DWORD_PTR	lParam;
        };
    public:
        //새 게임오브젝트를 동적할당해서 반환
        static GameObject* SpawnGameObject(define::eLayerType _layer);

        //기본적으로 자신을 포함한 Child 게임오브젝트를 동일한 레이어에 넣도록 설정하였음.
        //만약 특정 자식 오브젝트만 다른 레이어에 넣고 싶을 경우 MoveLayer 함수를 사용할 것
        static GameObject* SpawnGameObject(define::eLayerType _layer, GameObject* _gameObj);

        static void        MoveGameObjectLayer(define::eLayerType _layer, GameObject* _gameObj);

        static void DestroyGameObj(GameObject* _pObject);

        //게임 진행 중에는 반드시 이 함수를 통해서 child를 집어넣을것
        //새로운 게임오브젝트일 경우 반드시 SpawnGameObject도 해줘야 함
        static void AddChildGameObj(GameObject* _pParent, GameObject* _pChild);


        //프리팹(리소스)를 넣어서 게임오브젝트를 스폰시킬 경우 해당 게임오브젝트의 주소를 리턴해줌(추가적인 설정을 할수있게)	
        //static GameObject* SpawnPrefab2D(std::shared_ptr<Prefab> _Prefab, const float2& _vWorldPosXY);
        //static GameObject* SpawnPrefab(std::shared_ptr<Prefab> _Prefab, const float3& _vWorldPos);
        //static GameObject* SpawnPrefab(std::shared_ptr<Prefab> _Prefab);


    private:
        static bool Init();
        static void Release();

        static void Update();

        static void AddEvent(const tEvent& _event) { mEvents.push_back(_event); }
        static bool IsLevelChanged() { return mbLevelModified; }

        //lParam = GameObject Pointer
        //wParam = Layer Number
        static void SpawnNewGameObj(const tEvent& _event);

        //lParam = GameObject Pointer
        //wParam = None
        static void DestroyGameObj(const tEvent& _event);

        //lParam = Parent GameObject*
        //wParam = Child GameObject*
        //SpawnNewGameObj는 따로 해주지 않으므로 주의
        static void AddChildGameObj(const tEvent& _event);

        //lParam = GameObject Pointer
        //wParam = Target Layer
        static void MoveGameObjLayer(const tEvent& _event);

    private:
        static void ProcessEvent();
        static void ProcessLazyEvent();

    private:
        static std::vector<tEvent> mEvents;

        //한 프레임 기다렸다 처리해야 하는 이벤트는 여기에 저장
        static std::vector<tEvent> mLazyEvents;

        static bool           mbLevelModified;


    private:
        EventMgr() = delete;
        ~EventMgr() = delete;
    };
}


