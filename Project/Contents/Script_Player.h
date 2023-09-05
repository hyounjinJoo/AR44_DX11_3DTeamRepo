#pragma once
#include <Engine/IScript.h>

namespace mh
{
    class Script_Player :
        public IScript
    {
    public:
        Script_Player();
        virtual ~Script_Player();

        virtual void Update() override;

        virtual void OnCollisionEnter(ICollider2D* _collider) override {};
        virtual void OnCollisionStay(ICollider2D* _collider) override {};
        virtual void OnCollisionExit(ICollider2D* _collider) override {};

        void Start() {};
        void Action() {};
        void End() {};

    private:
        float mMoveSpeed;
    };
}

