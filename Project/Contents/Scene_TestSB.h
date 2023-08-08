#pragma once

#include <Engine/IScene.h>

namespace mh
{
    class Scene_TestSB :
        public IScene
    {
    public:
        Scene_TestSB();
        virtual ~Scene_TestSB();

        virtual void OnEnter() override {}

    private:
        int i;
    };
}


