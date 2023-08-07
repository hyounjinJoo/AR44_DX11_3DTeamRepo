#pragma once

#include <Engine/IScene.h>

namespace mh
{
    class Scene_TestCH :
        public IScene
    {
    public:
        Scene_TestCH();
        virtual ~Scene_TestCH();

        virtual void OnEnter() override {}

    private:
        int i;
    };
}


