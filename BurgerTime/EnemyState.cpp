#include "EnemyState.h"
#include "EnemyComponent.h"

#include <EngineTime.h>
#include <GameObject.h>
#include <BaseComponent.h>
#include <glm.hpp>

using namespace EnemyStates;

static constexpr auto InputToDirection(InputKey input) -> glm::vec2
{
    switch (input)
    {
    case InputKey::MoveLeft:
        return DirectionVec::Left;
    case InputKey::MoveRight:
        return DirectionVec::Right;
    case InputKey::MoveUp:
        return DirectionVec::Up;
    case InputKey::MoveDown:
        return DirectionVec::Down;
    default:
        return {};
    }
}

// IDLE STATE
void IdleState::Enter(EnemyComponent&) {}

void IdleState::Exit(EnemyComponent&) {}

auto IdleState::Update(EnemyComponent&) -> EnemyState& { return *this; }

auto IdleState::HandleInput(EnemyComponent& enemy, InputKey& input)
    -> EnemyState&
{
    switch (input)
    {
    case InputKey::MoveLeft:
    case InputKey::MoveRight:
    case InputKey::MoveUp:
    case InputKey::MoveDown:
    {
        if (enemy.Move(InputToDirection(input)))
            return enemy.GetMoveState();
        else
            return enemy.GetIdleState();
    }
    default:
        return *this;
    };
}

// MOVE STATE
void MoveState::Enter(EnemyComponent&) {}

void MoveState::Exit(EnemyComponent&) {}

auto MoveState::Update(EnemyComponent& enemy) -> EnemyState&
{
    if (enemy.HasMoved())
        return *this;

    return enemy.GetIdleState();
}

auto MoveState::HandleInput(EnemyComponent& enemy, InputKey& input)
    -> EnemyState&
{
    switch (input)
    {
    case InputKey::MoveUp:
    case InputKey::MoveDown:
    case InputKey::MoveLeft:
    case InputKey::MoveRight:
    {
        if (enemy.Move(InputToDirection(input)))
            return enemy.GetMoveState();
        else
            return enemy.GetIdleState();
    }
    default:
        return *this;
    }
}

// ATTACK STATE
void AttackState::Enter(EnemyComponent&) { m_AttackTimer = 0.0f; }

void AttackState::Exit(EnemyComponent&) {}

auto AttackState::Update(EnemyComponent& enemy) -> EnemyState&
{
    m_AttackTimer += dae::EngineTime::GetInstance().GetDeltaTime();
    if (m_AttackTimer < m_AttackDuration)
        return enemy.GetIdleState();
    return *this;
}

auto AttackState::HandleInput(EnemyComponent&, InputKey&) -> EnemyState&
{
    return *this;
}

// DIE STATE
void DieState::Enter(EnemyComponent&) { m_DeathTimer = 0.0f; }

void DieState::Exit(EnemyComponent&) {}

auto DieState::Update(EnemyComponent& enemy) -> EnemyState&
{
    m_DeathTimer += dae::EngineTime::GetInstance().GetDeltaTime();

    if (m_DeathTimer >= m_DeathDuration)
    {
        enemy.Die();
        return enemy.GetIdleState();
    }
    return *this;
}

auto DieState::HandleInput(EnemyComponent&, InputKey&) -> EnemyState&
{
    return *this;
}
