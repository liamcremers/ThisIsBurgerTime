#include "PlayerState.h"
#include "PlayerComponent.h"

#include <BaseComponent.h>
#include <GameObject.h>
#include "PlayerInputComponent.h"

void IdleState::Enter(PlayerComponent& player) { player.SetAnimation("Idle"); }

void IdleState::HandleInput(PlayerComponent&) {}

void IdleState::Update(PlayerComponent& player)
{
    //if (player.IsMoving())
    if (player.IsClimbing())
        player.ChangeState(std::make_unique<ClimbState>());
    else if (player.IsWalking())
        player.ChangeState(std::make_unique<WalkState>());
}

void IdleState::Exit(PlayerComponent&) {}

PlayerStateType IdleState::GetStateType() const { return PlayerStateType::Idle; }

void WalkState::Enter(PlayerComponent& player) { player.SetAnimation("Walk"); }

void WalkState::HandleInput(PlayerComponent&) {}

void WalkState::Update(PlayerComponent& player)
{
    if (not player.IsMoving())
        player.ChangeState(std::make_unique<IdleState>());
    else if (player.IsClimbing())
        player.ChangeState(std::make_unique<ClimbState>());
}

void WalkState::Exit(PlayerComponent&) {}

PlayerStateType WalkState::GetStateType() const { return PlayerStateType::Walk; }

void ClimbState::Enter(PlayerComponent& player)
{
    player.SetAnimation("Climb");
}

void ClimbState::HandleInput(PlayerComponent&) {}

void ClimbState::Update(PlayerComponent& player)
{
    if (player.IsWalking())
        player.ChangeState(std::make_unique<WalkState>());
}

void ClimbState::Exit(PlayerComponent&) {}

PlayerStateType ClimbState::GetStateType() const { return PlayerStateType::Climb; }

void AttackState::Enter(PlayerComponent& player)
{
    player.SetAnimation("Attack");
    player.SetCanMove(false);
}

void AttackState::HandleInput(PlayerComponent&) {}

void AttackState::Update(PlayerComponent&) {}

void AttackState::Exit(PlayerComponent&) {}

PlayerStateType AttackState::GetStateType() const { return PlayerStateType::Attack; }

void DieState::Enter(PlayerComponent& player)
{
    player.SetAnimation("Die");
    player.SetCanMove(false);
}

void DieState::HandleInput(PlayerComponent&) {}

void DieState::Update(PlayerComponent&) {}

void DieState::Exit(PlayerComponent&) {}

PlayerStateType DieState::GetStateType() const { return PlayerStateType::Die; }
