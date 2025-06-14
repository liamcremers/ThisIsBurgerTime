#include "PlayerState.h"
#include "PlayerComponent.h"

#include <EngineTime.h>
#include <GameObject.h>
#include <BaseComponent.h>
#include <glm.hpp>

using namespace PlayerStates;

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
void IdleState::Enter(PlayerComponent&) {}

void IdleState::Exit(PlayerComponent&) {}

auto IdleState::Update(PlayerComponent&) -> PlayerState& { return *this; }

auto IdleState::HandleInput(PlayerComponent& player, InputKey& input)
    -> PlayerState&
{
    switch (input)
    {
    case InputKey::MoveLeft:
    case InputKey::MoveRight:
    case InputKey::MoveUp:
    case InputKey::MoveDown:
        player.Move(InputToDirection(input));
        return player.GetMoveState();
    case InputKey::Attack:
        return player.GetAttackState();
    default:
        return *this;
    };
}

// MOVE STATE
void MoveState::Enter(PlayerComponent&) {}

void MoveState::Exit(PlayerComponent&) {}

auto MoveState::Update(PlayerComponent& player) -> PlayerState&
{
    if (player.HasMoved())
        return *this;

    return player.GetIdleState();
}

auto MoveState::HandleInput(PlayerComponent& player, InputKey& input)
    -> PlayerState&
{
    switch (input)
    {
    case InputKey::MoveUp:
    case InputKey::MoveDown:
    case InputKey::MoveLeft:
    case InputKey::MoveRight:
        player.Move(InputToDirection(input));
        return *this;
    case InputKey::Attack:
        return player.GetAttackState();
    default:
        return *this;
    }
}

// ATTACK STATE
void AttackState::Enter(PlayerComponent&) { m_AttackTimer = 0.0f; }

void AttackState::Exit(PlayerComponent&) {}

auto AttackState::Update(PlayerComponent& player) -> PlayerState&
{
    m_AttackTimer += dae::EngineTime::GetInstance().GetDeltaTime();
    if (m_AttackTimer < m_AttackDuration)
        return player.GetIdleState();
    return *this;
}

auto AttackState::HandleInput(PlayerComponent&, InputKey&) -> PlayerState&
{
    return *this;
}

// DIE STATE
void DieState::Enter(PlayerComponent&) { m_DeathTimer = 0.0f; }

void DieState::Exit(PlayerComponent&) {}

auto DieState::Update(PlayerComponent& player) -> PlayerState&
{
    m_DeathTimer += dae::EngineTime::GetInstance().GetDeltaTime();

    if (m_DeathTimer >= m_DeathDuration)
    {
        player.Die();
        return player.GetIdleState();
    }
    return *this;
}

auto DieState::HandleInput(PlayerComponent&, InputKey&) -> PlayerState&
{
    return *this;
}
