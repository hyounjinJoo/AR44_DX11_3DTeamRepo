#pragma once
#include "IComponent.h"
#include "BrainTree\BrainTree.h"

namespace mh
{
	class Com_BehaviorTree : public IComponent
	{
	public:
		Com_BehaviorTree();
		CLONE_DISABLE(Com_BehaviorTree);

		virtual ~Com_BehaviorTree();

		//virtual eResult SaveJson(Json::Value* _pJVal) override;
		//virtual eResult LoadJson(const Json::Value* _pJVal) override;

	public:
		virtual void Init() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;

#pragma region deletedFunction
	public:
		Com_BehaviorTree(const Com_BehaviorTree& _Other) = delete;
		Com_BehaviorTree(Com_BehaviorTree&& _Other) noexcept = delete;
		Com_BehaviorTree& operator=(const Com_BehaviorTree& _Other) = delete;
		Com_BehaviorTree& operator=(Com_BehaviorTree&& _Other) noexcept = delete;
#pragma endregion

	public:
		std::shared_ptr<BrainTree::BehaviorTree> GetBT() { return mTree; }
		std::shared_ptr<BrainTree::Blackboard> GetBlackBoard() { return mBlackBoard; }

		bool isSuccess() const { return mTree->isSuccess(); }
		bool isFailure() const { return mTree->isFailure(); }
		bool isRunning() const { return mTree->isRunning(); }
		bool isTerminated() const { return mTree->isTerminated(); }

		void reset() { mTree->reset(); }

		// Behavior Tree가 보유한 블랙보드에 접근할 수 있도록 한다.
		// 단, 실제 Node에서 블랙보드에 접근할 때는 blackboard 변수로부터 접근할 수 있다.
#pragma region BlackBoard Function
	public:
		void SetBoolToBB(std::string key, bool value) { mBlackBoard->setBool(key, value); }
		bool GetBoolFromBB(std::string key) { return mBlackBoard->getBool(key); }
		bool HasBoolInBB(std::string key) const { return mBlackBoard->hasBool(key); }

		void SetIntToBB(std::string key, int value) { mBlackBoard->setInt(key, value); }
		int GetIntFromBB(std::string key) { return mBlackBoard->getInt(key); }
		bool HasIntInBB(std::string key) const { return mBlackBoard->hasInt(key); }

		void SetFloatToBB(std::string key, float value) { mBlackBoard->setFloat(key, value); }
		float GetFloatFromBB(std::string key) { return mBlackBoard->getFloat(key); }
		bool HasFloatInBB(std::string key) const { return mBlackBoard->hasFloat(key); }

		void SetDoubleToBB(std::string key, double value) { mBlackBoard->setDouble(key, value); }
		double GetDoubleFromBB(std::string key) { return mBlackBoard->getDouble(key); }
		bool HasDoubleInBB(std::string key) const { return mBlackBoard->hasDouble(key); }

		void SetStringToBB(std::string key, std::string value) { mBlackBoard->setString(key, value); }
		const std::string& GetStringFromBB(std::string key) const { return mBlackBoard->getString(key); }
		bool HasStringInBB(std::string key) const { return mBlackBoard->hasString(key); }
#pragma endregion

	private:
		std::shared_ptr<BrainTree::BehaviorTree> mTree;
		std::shared_ptr<BrainTree::Blackboard> mBlackBoard;
	};
}
