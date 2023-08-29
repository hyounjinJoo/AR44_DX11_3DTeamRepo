#include "../BrainTree.h"
#include <Windows.h>

namespace BrainTree
{
    class SayHello : public Node
    {
        Status update() override
		{
			char Text[256];
			sprintf_s(Text, "Test\n");
			OutputDebugStringA(Text);
            return Node::Status::Success;
        }
    };

    int Example()
    {
        BehaviorTree tree;

        auto repeater = std::make_shared<Repeater>(5);
        repeater->setChild(std::make_shared<SayHello>());
        tree.setRoot(repeater);

        // simulate 5 frames
        for (int i = 0; i < 5; i++)
            tree.update();

        return 0;
    }
}
