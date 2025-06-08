#include "PlayerState.h"
#include "PlayerComponent.h"

#include <GameObject.h>
#include <BaseComponent.h>
#include <glm.hpp>

static constexpr glm::vec2 InputToDirection(PlayerInputKeys input)
{
    switch (input)
    {
    case PlayerInputKeys::MoveLeft:
        return DirectionVec::Left;
    case PlayerInputKeys::MoveRight:
        return DirectionVec::Right;
    case PlayerInputKeys::MoveUp:
        return DirectionVec::Up;
    case PlayerInputKeys::MoveDown:
        return DirectionVec::Down;
    default:
        return {};
    }
}

// IDLE STATE
void IdleState::Enter(PlayerComponent&) {}

void IdleState::Exit(PlayerComponent&) {}

PlayerState& IdleState::Update(PlayerComponent&) { return *this; }

PlayerState& IdleState::HandleInput(PlayerComponent& player,
                                    PlayerInputKeys& input)
{
    switch (input)
    {
    case PlayerInputKeys::MoveLeft:
    case PlayerInputKeys::MoveRight:
    case PlayerInputKeys::MoveUp:
    case PlayerInputKeys::MoveDown:
        player.OnMove(InputToDirection(input));
        return player.GetMoveState();
    case PlayerInputKeys::Attack:
        return player.GetAttackState();
#ifdef _DEBUG
    case PlayerInputKeys::Die:
        return player.GetDieState();
#endif // _DEBUG
    default:
        return *this;
    };
}

// MOVE STATE
void MoveState::Enter(PlayerComponent&) {}

void MoveState::Exit(PlayerComponent&) {}

PlayerState& MoveState::Update(PlayerComponent& player)
{
    if (player.HasMoved())
        return *this;

    return player.GetIdleState();
}

PlayerState& MoveState::HandleInput(PlayerComponent& player,
                                    PlayerInputKeys& input)
{
    switch (input)
    {
    case PlayerInputKeys::MoveUp:
    case PlayerInputKeys::MoveDown:
    case PlayerInputKeys::MoveLeft:
    case PlayerInputKeys::MoveRight:
        player.OnMove(InputToDirection(input));
        return *this;
    case PlayerInputKeys::Attack:
        return player.GetAttackState();
#ifdef _DEBUG
    case PlayerInputKeys::Die:
        return player.GetDieState();
#endif // _DEBUG
    default:
        return *this;
    }
}

// ATTACK STATE
void AttackState::Enter(PlayerComponent&) { m_AttackTimer = 0.0f; }

void AttackState::Exit(PlayerComponent&) {}

PlayerState& AttackState::Update(PlayerComponent& player)
{
    m_AttackTimer += dae::EngineTime::GetInstance().GetDeltaTime();
    if (m_AttackTimer < m_AttackDuration)
        return player.GetIdleState();
    return *this;
}

PlayerState& AttackState::HandleInput(PlayerComponent&, PlayerInputKeys&)
{
    return *this;
}

// DIE STATE
void DieState::Enter(PlayerComponent&) { m_DeathTimer = 0.0f; }

void DieState::Exit(PlayerComponent&) {}

PlayerState& DieState::Update(PlayerComponent& player)
{
    m_DeathTimer += dae::EngineTime::GetInstance().GetDeltaTime();

    if (m_DeathTimer >= m_DeathDuration)
    {
        player.OnDeath();
        return player.GetIdleState();
    }
    return *this;
}

PlayerState& DieState::HandleInput(PlayerComponent&, PlayerInputKeys&)
{
    return *this;
}
