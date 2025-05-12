#pragma once
#include <GameObject.h>

class PlayerComponent;

enum class PlayerStateType
{
    Idle,
    Walk,
    Climb,
    Attack,
    Die
};

class PlayerState
{
public:
    virtual ~PlayerState() = default;
    virtual void Enter(PlayerComponent& player) = 0;
    virtual void Update(PlayerComponent& player) = 0;
    virtual void HandleInput(PlayerComponent& player) = 0;
    virtual void Exit(PlayerComponent& player) = 0;
    virtual [[nodiscard]] PlayerStateType GetStateType() const = 0;
};

class IdleState : public PlayerState
{
public:
    virtual ~IdleState() = default;
    void Enter(PlayerComponent& player) override;
    void HandleInput(PlayerComponent& player) override;
    void Update(PlayerComponent& player) override;
    void Exit(PlayerComponent& player) override;
    [[nodiscard]] PlayerStateType GetStateType() const override;
};

class WalkState : public PlayerState
{
public:
    virtual ~WalkState() = default;
    void Enter(PlayerComponent& player) override;
    void HandleInput(PlayerComponent& player) override;
    void Update(PlayerComponent& player) override;
    void Exit(PlayerComponent& player) override;

    WalkState() = default;
    [[nodiscard]] PlayerStateType GetStateType() const override;
};

class ClimbState : public PlayerState
{
public:
    virtual ~ClimbState() = default;
    void Enter(PlayerComponent& player) override;
    void HandleInput(PlayerComponent& player) override;
    void Update(PlayerComponent& player) override;
    void Exit(PlayerComponent& player) override;
    [[nodiscard]] PlayerStateType GetStateType() const override;
};

class AttackState : public PlayerState
{
public:
    virtual ~AttackState() = default;
    void Enter(PlayerComponent& player) override;
    void HandleInput(PlayerComponent& player) override;
    void Update(PlayerComponent& player) override;
    void Exit(PlayerComponent& player) override;
    [[nodiscard]] PlayerStateType GetStateType() const override;
};

class DieState : public PlayerState
{
public:
    virtual ~DieState() = default;
    void Enter(PlayerComponent& player) override;
    void HandleInput(PlayerComponent& player) override;
    void Update(PlayerComponent& player) override;
    void Exit(PlayerComponent& player) override;
    [[nodiscard]] PlayerStateType GetStateType() const override;
};
