#include "PlayerState.h"
#include "PlayerComponent.h"

#include <GameObject.h>
#include <BaseComponent.h>
#include <glm.hpp>

// IDLE STATE
void IdleState::Enter(PlayerComponent&) {}

void IdleState::Exit(PlayerComponent&) {}

PlayerState& IdleState::Update(PlayerComponent& player)
{
    return player.GetIdleState();
}

PlayerState& IdleState::HandleInput(PlayerComponent& player,
                                    PlayerInputKeys& input)
{
    switch (input)
    {
    case PlayerInputKeys::MoveLeft:
        player.Move(PlayerDirection::Left);
    case PlayerInputKeys::MoveRight:
        player.Move(PlayerDirection::Right);
    case PlayerInputKeys::MoveUp:
        player.Move(PlayerDirection::Up);
    case PlayerInputKeys::MoveDown:
        player.Move(PlayerDirection::Down);
    };

    switch (input)
    {
    case PlayerInputKeys::MoveLeft:
    case PlayerInputKeys::MoveRight:
    case PlayerInputKeys::MoveUp:
    case PlayerInputKeys::MoveDown:
        return player.GetMoveState();
    case PlayerInputKeys::Attack:
        return player.GetAttackState();
    default:
        return player.GetIdleState();
    }
}

// MOVE STATE
void MoveState::Enter(PlayerComponent&) {}

void MoveState::Exit(PlayerComponent&) {}

PlayerState& MoveState::Update(PlayerComponent& player)
{
    if (!player.HasMoved())
    {

        return player.GetIdleState();
    }

    return player.GetMoveState();
}

PlayerState& MoveState::HandleInput(PlayerComponent& player,
                                    PlayerInputKeys& input)
{
    switch (input)
    {
    case PlayerInputKeys::MoveUp:
        player.Move(PlayerDirection::Up);
        return player.GetMoveState();
    case PlayerInputKeys::MoveDown:
        player.Move(PlayerDirection::Down);
        return player.GetMoveState();
    case PlayerInputKeys::MoveLeft:
        player.Move(PlayerDirection::Left);
        return player.GetMoveState();
    case PlayerInputKeys::MoveRight:
        player.Move(PlayerDirection::Right);
        return player.GetMoveState();
    case PlayerInputKeys::Attack:
        return player.GetAttackState();
    default:
        return player.GetIdleState();
    }
}

// ATTACK STATE
void AttackState::Enter(PlayerComponent&) {}

void AttackState::Exit(PlayerComponent&) {}

PlayerState& AttackState::Update(PlayerComponent& player)
{
    return player.GetAttackState();
}

PlayerState& AttackState::HandleInput(PlayerComponent& player, PlayerInputKeys&)
{
    return player.GetAttackState();
}

// DIE STATE
void DieState::Enter(PlayerComponent&) {}

void DieState::Exit(PlayerComponent&) {}

PlayerState& DieState::Update(PlayerComponent& player)
{
    return player.GetDieState();
}

PlayerState& DieState::HandleInput(PlayerComponent& player, PlayerInputKeys&)
{
    return player.GetDieState();
}
