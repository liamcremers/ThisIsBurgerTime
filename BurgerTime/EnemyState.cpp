#include "EnemyState.h"
#include "EnemyComponent.h"

#include <EngineTime.h>
#include <GameObject.h>
#include <BaseComponent.h>
#include <glm.hpp>

using namespace EnemyComp;
using namespace EnemyStates;

static constexpr auto InputToDirection(EnemyInputKeys input) -> glm::vec2
{
    switch (input)
    {
    case EnemyInputKeys::MoveLeft:
        return DirectionVec::Left;
    case EnemyInputKeys::MoveRight:
        return DirectionVec::Right;
    case EnemyInputKeys::MoveUp:
        return DirectionVec::Up;
    case EnemyInputKeys::MoveDown:
        return DirectionVec::Down;
    default:
        return {};
    }
}

// IDLE STATE
void IdleState::Enter(EnemyComponent&) {}

void IdleState::Exit(EnemyComponent&) {}

auto IdleState::Update(EnemyComponent&) -> EnemyState& { return *this; }

auto IdleState::HandleInput(EnemyComponent& enemy, EnemyInputKeys& input)
    -> EnemyState&
{
    switch (input)
    {
    case EnemyInputKeys::MoveLeft:
    case EnemyInputKeys::MoveRight:
    case EnemyInputKeys::MoveUp:
    case EnemyInputKeys::MoveDown:
        enemy.OnMove(InputToDirection(input));
        return enemy.GetMoveState();
#ifdef _DEBUG
    case EnemyInputKeys::Attack:
        return enemy.GetAttackState();
    case EnemyInputKeys::Die:
        return enemy.GetDieState();
#endif // _DEBUG
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

auto MoveState::HandleInput(EnemyComponent& enemy, EnemyInputKeys& input)
    -> EnemyState&
{
    switch (input)
    {
    case EnemyInputKeys::MoveUp:
    case EnemyInputKeys::MoveDown:
    case EnemyInputKeys::MoveLeft:
    case EnemyInputKeys::MoveRight:
        enemy.OnMove(InputToDirection(input));
        return *this;
#ifdef _DEBUG
    case EnemyInputKeys::Attack:
        return enemy.GetAttackState();
    case EnemyInputKeys::Die:
        return enemy.GetDieState();
#endif // _DEBUG
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

auto AttackState::HandleInput(EnemyComponent&, EnemyInputKeys&) -> EnemyState&
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
        enemy.OnDeath();
        return enemy.GetIdleState();
    }
    return *this;
}

auto DieState::HandleInput(EnemyComponent&, EnemyInputKeys&) -> EnemyState&
{
    return *this;
}
