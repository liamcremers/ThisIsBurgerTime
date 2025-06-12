#pragma once
#include <glm.hpp>

namespace BurgerComp
{
    class BurgerPartComponent;
}

using BurgerPartComponent = BurgerComp::BurgerPartComponent;

namespace BurgerStates
{
    class BurgerState
    {
    public:
        virtual ~BurgerState() = default;
        virtual void Enter(BurgerPartComponent&) = 0;
        virtual void Exit(BurgerPartComponent&) = 0;
        virtual BurgerState& Update(BurgerPartComponent& enemy) = 0;
    };

    class IdleState : public BurgerState
    {
    public:
        void Enter(BurgerPartComponent&) override;

        void Exit(BurgerPartComponent&) override {}

        BurgerState& Update(BurgerPartComponent& enemy) override;
    };

    class WalkedOnState : public BurgerState
    {
    public:
        void Enter(BurgerPartComponent&) override;

        void Exit(BurgerPartComponent&) override {}

        BurgerState& Update(BurgerPartComponent& enemy) override;
    };

    class FallingState : public BurgerState
    {
    public:
        void Enter(BurgerPartComponent&) override;

        void Exit(BurgerPartComponent&) override {}

        BurgerState& Update(BurgerPartComponent& enemy) override;

    private:
        int m_FallAmount{ 1 };
        bool m_HasFallenOnce{};
    };
}
