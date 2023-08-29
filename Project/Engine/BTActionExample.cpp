#include "PCH_Engine.h"
#include "BTActionExample.h"

namespace mh
{
	// There is no need to call Parent's Constructor.
	BTActionExample::BTActionExample()
	{
	}

	BTActionExample::BTActionExample(std::shared_ptr<BrainTree::Blackboard> blackboard)
		: Leaf(blackboard)
	{
	}

	BTActionExample::~BTActionExample()
	{
	}

	BrainTree::Node::Status BTActionExample::update()
	{
		// There is no need to Call Parent's update() function.
		// Parent's update() is pure virtual function.
		// this is main logic function.
		return BrainTree::Node::Status::Success;
	}
}
