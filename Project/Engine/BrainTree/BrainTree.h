// BrainTree - A C++ behavior tree single header library.
// Copyright 2015-2018 Par Arvidsson. All rights reserved.
// Licensed under the MIT license (https://github.com/arvidsson/BrainTree/blob/master/LICENSE).

// 주석 작성 : JHJ
// 작성 일자 : 230828 ~ 230829
#pragma once

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <cassert>

namespace BrainTree
{
#pragma region Base
    // 각 노드(Behaviour Tree, Composite, Sequence 등)의 최상위 부모 클래스
    // Status enum class를 통해 해당 노드의 수행 결과가 어떻게 되었는지를 관리하는 것을 주요한 목적으로 한다.
    class Node
    {
    public:
        // 노드의 수행 결과를 나타낸다.
        enum class Status
        {
            Invalid,        // 초기 상태
            Success,        // 성공
            Failure,        // 실패
            Running,        // 수행 중
        };

        virtual ~Node() {}

        // 하위 노드에서 update()의 경우 무조건 재정의 필요
        virtual Status update() = 0;
        virtual void initialize() {}
        virtual void terminate(Status s) {}

        // 행동 트리의 수행결과를 반환한다.
        Status tick()
        {
            if (status != Status::Running) {
                initialize();
            }

            status = update();

            if (status != Status::Running) {
                terminate(status);
            }

            return status;
        }

        // Status Getter
        bool isSuccess() const { return status == Status::Success; }
        bool isFailure() const { return status == Status::Failure; }
        bool isRunning() const { return status == Status::Running; }
        bool isTerminated() const { return isSuccess() || isFailure(); }

        // 초기 상태로 노드를 변경한다.
        void reset() { status = Status::Invalid; }

        // std::shared_ptr<Node> 타입지정, template 사용을 위해서 using으로 선언
        using Ptr = std::shared_ptr<Node>;

    protected:
        Status status = Status::Invalid;
    };

    // Composite 노드의 Base 클래스
    class Composite : public Node
    {
    public:
        Composite() : it(children.begin()) {}
        virtual ~Composite() {}

        // 하위에 추가될 Node가 추가된다. 각 노드는 Action이 될 수도 다른 Composite가 될 수도, Decorator가 될 수도 있다.
        void addChild(Node::Ptr child) { children.push_back(child); }
        bool hasChildren() const { return !children.empty(); }

    protected:
        std::vector<Node::Ptr> children;
        std::vector<Node::Ptr>::iterator it;
    };

    // Decorator는 반복, 성공할 때까지 반복, 실패할 때까지 반복 등의 하위 Node의 동작결과를
    // 수식하는 역할을 한다.
    class Decorator : public Node
    {
    public:
        virtual ~Decorator() {}

        void setChild(Node::Ptr node) { child = node; }
        bool hasChild() const { return child != nullptr; }

    protected:
        Node::Ptr child = nullptr;
    };
#pragma endregion

    // Behavior Tree에서 각 노드가 공유할 수 있는 데이터를 담는 역할을 한다.
    class Blackboard
    {
    public:
        void setBool(std::string key, bool value) { bools[key] = value; }
        bool getBool(std::string key)
        {
            if (bools.find(key) == bools.end()) {
                bools[key] = false;
            }
            return bools[key];
        }
        bool hasBool(std::string key) const { return bools.find(key) != bools.end(); }

        void setInt(std::string key, int value) { ints[key] = value; }
        int getInt(std::string key)
        {
            if (ints.find(key) == ints.end()) {
                ints[key] = 0;
            }
            return ints[key];
        }
        bool hasInt(std::string key) const { return ints.find(key) != ints.end(); }

        void setFloat(std::string key, float value) { floats[key] = value; }
        float getFloat(std::string key)
        {
            if (floats.find(key) == floats.end()) {
                floats[key] = 0.0f;
            }
            return floats[key];
        }
        bool hasFloat(std::string key) const { return floats.find(key) != floats.end(); }

        void setDouble(std::string key, double value) { doubles[key] = value; }
        double getDouble(std::string key)
        {
            if (doubles.find(key) == doubles.end()) {
                doubles[key] = 0.0f;
            }
            return doubles[key];
        }
        bool hasDouble(std::string key) const { return doubles.find(key) != doubles.end(); }

        void setString(std::string key, std::string value) { strings[key] = value; }
        std::string getString(std::string key)
        {
            if (strings.find(key) == strings.end()) {
                strings[key] = "";
            }
            return strings[key];
        }
        bool hasString(std::string key) const { return strings.find(key) != strings.end(); }

        using Ptr = std::shared_ptr<Blackboard>;

    protected:
        std::unordered_map<std::string, bool> bools;
        std::unordered_map<std::string, int> ints;
        std::unordered_map<std::string, float> floats;
        std::unordered_map<std::string, double> doubles;
        std::unordered_map<std::string, std::string> strings;
    };

    // 실제 동작을 나타낼 노드. 재정의하여 사용하면 된다.
    // update() 가 실제 동작이 된다.
    class Leaf : public Node
    {
    public:
        Leaf() {}
        virtual ~Leaf() {}
        Leaf(Blackboard::Ptr blackboard) : blackboard(blackboard) {}

        virtual Status update() = 0;

    protected:
        Blackboard::Ptr blackboard;
    };

    // BehaviorTree를 나타낸다. 반드시 root가 설정되어야 한다.
    class BehaviorTree : public Node
    {
    public:
        BehaviorTree() : blackboard(std::make_shared<Blackboard>()) {}
        BehaviorTree(const Node::Ptr& rootNode) : BehaviorTree() { root = rootNode; }

        Status update() { return root->tick(); }

        Node::Ptr getRoot() { return root; }
        void setRoot(const Node::Ptr& node) { root = node; }
        Blackboard::Ptr getBlackboard() const { return blackboard; }

    private:
        Node::Ptr root = nullptr;
        Blackboard::Ptr blackboard = nullptr;
    };

#pragma region Builder
    template <class Parent>
    class DecoratorBuilder;

    template <class Parent>
    class CompositeBuilder
    {
    public:
        CompositeBuilder(Parent* parent, Composite* node) : parent(parent), node(node) {}

        template <class NodeType, typename... Args>
        CompositeBuilder<Parent> leaf(Args... args)
        {
            auto child = std::make_shared<NodeType>((args)...);
            node->addChild(child);
            return *this;
        }

        template <class CompositeType, typename... Args>
        CompositeBuilder<CompositeBuilder<Parent>> composite(Args... args)
        {
            auto child = std::make_shared<CompositeType>((args)...);
            node->addChild(child);
            return CompositeBuilder<CompositeBuilder<Parent>>(this, (CompositeType*)child.get());
        }

        template <class DecoratorType, typename... Args>
        DecoratorBuilder<CompositeBuilder<Parent>> decorator(Args... args)
        {
            auto child = std::make_shared<DecoratorType>((args)...);
            node->addChild(child);
            return DecoratorBuilder<CompositeBuilder<Parent>>(this, (DecoratorType*)child.get());
        }

        Parent& end()
        {
            return *parent;
        }

    private:
        Parent* parent;
        Composite* node;
    };

    template <class Parent>
    class DecoratorBuilder
    {
    public:
        DecoratorBuilder(Parent* parent, Decorator* node) : parent(parent), node(node) {}

        template <class NodeType, typename... Args>
        DecoratorBuilder<Parent> leaf(Args... args)
        {
            auto child = std::make_shared<NodeType>((args)...);
            node->setChild(child);
            return *this;
        }

        template <class CompositeType, typename... Args>
        CompositeBuilder<DecoratorBuilder<Parent>> composite(Args... args)
        {
            auto child = std::make_shared<CompositeType>((args)...);
            node->setChild(child);
            return CompositeBuilder<DecoratorBuilder<Parent>>(this, (CompositeType*)child.get());
        }

        template <class DecoratorType, typename... Args>
        DecoratorBuilder<DecoratorBuilder<Parent>> decorator(Args... args)
        {
            auto child = std::make_shared<DecoratorType>((args)...);
            node->setChild(child);
            return DecoratorBuilder<DecoratorBuilder<Parent>>(this, (DecoratorType*)child.get());
        }

        Parent& end()
        {
            return *parent;
        }

    private:
        Parent* parent;
        Decorator* node;
    };

    class Builder
    {
    public:
        template <class NodeType, typename... Args>
        Builder leaf(Args... args)
        {
            root = std::make_shared<NodeType>((args)...);
            return *this;
        }

        template <class CompositeType, typename... Args>
        CompositeBuilder<Builder> composite(Args... args)
        {
            root = std::make_shared<CompositeType>((args)...);
            return CompositeBuilder<Builder>(this, (CompositeType*)root.get());
        }

        template <class DecoratorType, typename... Args>
        DecoratorBuilder<Builder> decorator(Args... args)
        {
            root = std::make_shared<DecoratorType>((args)...);
            return DecoratorBuilder<Builder>(this, (DecoratorType*)root.get());
        }

        Node::Ptr build()
        {
            assert(root != nullptr && "The Behavior Tree is empty!");
            auto tree = std::make_shared<BehaviorTree>();
            tree->setRoot(root);
            return tree;
        }

    private:
        Node::Ptr root;
    };
#pragma endregion
#pragma region Composite
    // The Selector composite ticks each child node in order.
    // If a child succeeds or runs, the selector returns the same status.
    // In the next tick, it will try to run each child in order again.
    // If all children fails, only then does the selector fail.
    // Selector composite는 각 하위 노드를 순서대로 선택(실행)한다.
    // 하위 노드가 성공하거나 동작 중인 상태라면, Selector는 그 상태를 반환한다.
    // 다음 tick에서, Selector는 각 하위 노드를 순서대로 실행하려고 시도한다.
    // 모든 하위 노드들이 실패한 경우에만 Selector가 실패한다.
    class Selector : public Composite
    {
    public:
        void initialize() override
        {
            it = children.begin();
        }

        Status update() override
        {
            assert(hasChildren() && "Composite has no children");

            while (it != children.end()) {
                auto status = (*it)->tick();

                if (status != Status::Failure) {
                    return status;
                }

                it++;
            }

            return Status::Failure;
        }
    };

    // The Sequence composite ticks each child node in order.
    // If a child fails or runs, the sequence returns the same status.
    // In the next tick, it will try to run each child in order again.
	// If all children succeeds, only then does the sequence succeed.
	// Sequence composite는 각 하위 노드를 순서대로 선택(실행)한다.
	// 하위 노드가 실패하거나 동작 중인 상태라면, Sequence는 그 상태를 반환한다.
	// 다음 tick에서, Sequence는 각 하위 노드를 순서대로 실행하려고 시도한다.
	// 모든 하위 노드들이 성공한 경우에만 Sequence가 성공한다.
    class Sequence : public Composite
    {
    public:
        void initialize() override
        {
            it = children.begin();
        }

        Status update() override
        {
            assert(hasChildren() && "Composite has no children");

            while (it != children.end()) {
                auto status = (*it)->tick();

                if (status != Status::Success) {
                    return status;
                }

                it++;
            }

            return Status::Success;
        }
    };

    // The StatefulSelector composite ticks each child node in order, and remembers what child it prevously tried to tick.
    // If a child succeeds or runs, the stateful selector returns the same status.
    // In the next tick, it will try to run the next child or start from the beginning again.
    // If all children fails, only then does the stateful selector fail.
    // StatefulSelector(상태 저장 선택기) Composite는 각 하위 노드를 순서대로 tick(실행)하고 이전에 tick하려고 했던 하위 노드를 기억한다.
    // 하위 노드가 성공하거나 실행되면 StatefulSelector는 하위노드의 반환값과 동일한 상태값을 반환한다.
    // 다음 tick에서는 다음 하위 노드를 실행하거나 처음부터 다시 시작하려고 시도한다.
    // 모든 하위 노드가 실패하면 StatefulSelector가 실패한다.
    class StatefulSelector : public Composite
    {
    public:
        Status update() override
        {
            assert(hasChildren() && "Composite has no children");

            while (it != children.end()) {
                auto status = (*it)->tick();

                if (status != Status::Failure) {
                    return status;
                }

                it++;
            }

            it = children.begin();
            return Status::Failure;
        }
    };

    // The StatefulSequence composite ticks each child node in order, and remembers what child it prevously tried to tick.
    // If a child fails or runs, the stateful sequence returns the same status.
    // In the next tick, it will try to run the next child or start from the beginning again.
    // If all children succeeds, only then does the stateful sequence succeed.
	// StatefulSequence(상태 저장 시퀀스) Composite는 각 하위 노드를 순서대로 tick(실행)하고 이전에 tick하려고 했던 하위 노드를 기억한다.
	// 하위 노드가 실패하거나 실행되면 StatefulSequence는 하위노드의 반환값과 동일한 상태값을 반환한다.
	// 다음 tick에서는 다음 하위 노드를 실행하거나 처음부터 다시 시작하려고 시도한다.
	// 모든 하위 노드가 성공하면 StatefulSequence가 성공한다.
    class MemSequence : public Composite
    {
    public:
        Status update() override
        {
            assert(hasChildren() && "Composite has no children");

            while (it != children.end()) {
                auto status = (*it)->tick();

                if (status != Status::Success) {
                    return status;
                }

                it++;
            }

            it = children.begin();
            return Status::Success;
        }
    };

    // 최소 성공 수, 최소 실패 수, 모두 성공, 모두 실패와 같은 조건을 설정하여
    // 시퀀스의 수행 결과를 반환하는 시퀀스
    class ParallelSequence : public Composite
    {
    public:
        ParallelSequence(bool successOnAll = true, bool failOnAll = true) : useSuccessFailPolicy(true), successOnAll(successOnAll), failOnAll(failOnAll) {}
        ParallelSequence(int minSuccess, int minFail) : minSuccess(minSuccess), minFail(minFail) {}

        Status update() override
        {
            assert(hasChildren() && "Composite has no children");

            int minimumSuccess = minSuccess;
            int minimumFail = minFail;

            if (useSuccessFailPolicy) {
                if (successOnAll) {
                    minimumSuccess = children.size();
                }
                else {
                    minimumSuccess = 1;
                }

                if (failOnAll) {
                    minimumFail = children.size();
                }
                else {
                    minimumFail = 1;
                }
            }

            int total_success = 0;
            int total_fail = 0;

            for (auto& child : children) {
                auto status = child->tick();
                if (status == Status::Success) {
                    total_success++;
                }
                if (status == Status::Failure) {
                    total_fail++;
                }
            }

            if (total_success >= minimumSuccess) {
                return Status::Success;
            }
            if (total_fail >= minimumFail) {
                return Status::Failure;
            }

            return Status::Running;
        }

    private:
        bool useSuccessFailPolicy = false;
        bool successOnAll = true;
        bool failOnAll = true;
        int minSuccess = 0;
        int minFail = 0;
    };
#pragma endregion
#pragma region Decorator
    // The Succeeder decorator returns success, regardless of what happens to the child.
    // Succeeder decorator는 하위 노드의 수행결과를 무시하고 Success 상태를 반환한다.
    class Succeeder : public Decorator
    {
    public:
        Status update() override
        {
            child->tick();
            return Status::Success;
        }
    };

    // The Failer decorator returns failure, regardless of what happens to the child.
    // Failer decorator는 하위 노드의 수행결과를 무시하고 Failure 상태를 반환한다.
    class Failer : public Decorator
    {
    public:
        Status update() override
        {
            child->tick();
            return Status::Failure;
        }
    };

    // The Inverter decorator inverts the child node's status, i.e. failure becomes success and success becomes failure.
    // If the child runs, the Inverter returns the status that it is running too.
    // 하위 노드의 수행값과 반대되는 상태를 반환한다.
    // 단, 하위 노드가 수행 중인 상태라면 수행 중인 상태(run)을 반환한다.
    class Inverter : public Decorator
    {
    public:
        Status update() override
        {
            auto s = child->tick();

            if (s == Status::Success) {
                return Status::Failure;
            }
            else if (s == Status::Failure) {
                return Status::Success;
            }

            return s;
        }
    };

    // The Repeater decorator repeats infinitely or to a limit until the child returns success.
    // Repeater decorator는 하위 노드가 성공을 반환할 때까지 무한히 반복하거나
    // 제한된 실행 수에 도달할 때까지 하위 노드를 수행한다.
    class Repeater : public Decorator
    {
    public:
        Repeater(int limit = 0) : limit(limit) {}

        void initialize() override
        {
            counter = 0;
        }

        Status update() override
        {
            child->tick();

            if (limit > 0 && ++counter == limit) {
                return Status::Success;
            }

            return Status::Running;
        }

    protected:
        int limit;
        int counter = 0;
    };

    // The UntilSuccess decorator repeats until the child returns success and then returns success.
    // 하위노드가 Success를 반환할 때까지 수행한다.
    class UntilSuccess : public Decorator
    {
    public:
        Status update() override
        {
            while (1) {
                auto status = child->tick();

                if (status == Status::Success) {
                    return Status::Success;
                }
            }
        }
    };

    // The UntilFailure decorator repeats until the child returns fail and then returns success.
    // 하위노드가 Failure를 반환할 때까지 수행한다.
    class UntilFailure : public Decorator
    {
    public:
        Status update() override
        {
            while (1) {
                auto status = child->tick();

                if (status == Status::Failure) {
                    return Status::Success;
                }
            }
        }
    };
#pragma endregion
} // namespace BrainTree
