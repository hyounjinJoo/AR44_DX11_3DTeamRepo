#pragma once

#include <Engine/IScene.h>

namespace mh
{
    class Scene_TestJH :
        public IScene
    {
    public:
        Scene_TestJH();
        virtual ~Scene_TestJH();

        virtual void OnEnter() override {}

    private:
        int i;
    };
}


