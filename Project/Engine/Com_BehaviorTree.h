#pragma once

namespace mh
{
	class Com_BehaviorTree
	{
	public:
		// construtor destructor
		Com_BehaviorTree();
		~Com_BehaviorTree();

		// delete Function
		Com_BehaviorTree(const Com_BehaviorTree& _Other) = delete;
		Com_BehaviorTree(Com_BehaviorTree&& _Other) noexcept = delete;
		Com_BehaviorTree& operator=(const Com_BehaviorTree& _Other) = delete;
		Com_BehaviorTree& operator=(Com_BehaviorTree&& _Other) noexcept = delete;

	protected:


	private:

	};
}
