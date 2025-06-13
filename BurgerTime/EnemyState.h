#pragma once
#include "IControllable.h"
#include "Direction.h"
#include <glm.hpp>

class EnemyComponent;

namespace EnemyStates
{
    class EnemyState
    {
    public:
        virtual ~EnemyState() = default;
        virtual void Enter(EnemyComponent&) = 0;
        virtual void Exit(EnemyComponent&) = 0;
        virtual EnemyState& Update(EnemyComponent& enemy) = 0;
        virtual EnemyState& HandleInput(EnemyComponent& enemy,
                                        InputKey& input) = 0;
    };

    class IdleState : public EnemyState
    {
    public:
        virtual ~IdleState() = default;

        void Enter(EnemyComponent&) override;
        void Exit(EnemyComponent&) override;
        EnemyState& Update(EnemyComponent& enemy) override;
        EnemyState& HandleInput(EnemyComponent& enemy,
                                InputKey& input) override;
    };

    class MoveState : public EnemyState
    {
    public:
        virtual ~MoveState() = default;
        void Enter(EnemyComponent&) override;
        void Exit(EnemyComponent&) override;
        EnemyState& Update(EnemyComponent& enemy) override;
        EnemyState& HandleInput(EnemyComponent& enemy,
                                InputKey& input) override;
    };

    class AttackState : public EnemyState
    {
    public:
        virtual ~AttackState() = default;
        void Enter(EnemyComponent&) override;
        void Exit(EnemyComponent&) override;
        EnemyState& Update(EnemyComponent& enemy) override;
        EnemyState& HandleInput(EnemyComponent& enemy,
                                InputKey& input) override;

    private:
        float m_AttackTimer{};
        const float m_AttackDuration{ 0.5f };
    };

    class DieState : public EnemyState
    {
    public:
        virtual ~DieState() = default;
        void Enter(EnemyComponent&) override;
        void Exit(EnemyComponent&) override;
        EnemyState& Update(EnemyComponent& enemy) override;
        EnemyState& HandleInput(EnemyComponent& enemy, InputKey&) override;

    private:
        float m_DeathTimer{};
        const float m_DeathDuration{ 1.0f };
    };
}
