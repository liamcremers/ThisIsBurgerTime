#pragma once
#include "PlayerInputKeys.h"
#include <glm.hpp>

class PlayerComponent;

class PlayerState
{
public:
    virtual ~PlayerState() = default;
    virtual void Enter(PlayerComponent&) = 0;
    virtual void Exit(PlayerComponent&) = 0;
    virtual PlayerState& Update(PlayerComponent& player) = 0;
    virtual PlayerState& HandleInput(PlayerComponent& player,
                                     PlayerInputKeys& input) = 0;
};

class IdleState : public PlayerState
{
public:
    virtual ~IdleState() = default;
    void Enter(PlayerComponent&) override;
    void Exit(PlayerComponent&) override;
    PlayerState& Update(PlayerComponent& player) override;
    PlayerState& HandleInput(PlayerComponent& player,
                             PlayerInputKeys& input) override;
};

class MoveState : public PlayerState
{
public:
    virtual ~MoveState() = default;
    void Enter(PlayerComponent&) override;
    void Exit(PlayerComponent&) override;
    PlayerState& Update(PlayerComponent& player) override;
    PlayerState& HandleInput(PlayerComponent& player,
                             PlayerInputKeys& input) override;
};

class AttackState : public PlayerState
{
public:
    virtual ~AttackState() = default;
    void Enter(PlayerComponent&) override;
    void Exit(PlayerComponent&) override;
    PlayerState& Update(PlayerComponent& player) override;
    PlayerState& HandleInput(PlayerComponent& player,
                             PlayerInputKeys& input) override;

private:
    float m_AttackTimer{};
    const float m_AttackDuration{ 0.5f };
};

class DieState : public PlayerState
{
public:
    virtual ~DieState() = default;
    void Enter(PlayerComponent&) override;
    void Exit(PlayerComponent&) override;
    PlayerState& Update(PlayerComponent& player) override;
    PlayerState& HandleInput(PlayerComponent& player,
                             PlayerInputKeys&) override;

private:
    float m_DeathTimer{};
    const float m_DeathDuration{ 2.0f };
};
