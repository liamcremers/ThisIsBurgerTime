#pragma once
#include "PlayerState.h"
#include "GridMoveCommand.h"
#include <GameObject.h>
#include <BaseComponent.h>

#include <memory>
#include <glm.hpp>

struct PlayerDirection
{
    static constexpr glm::vec2 Left{ -1, 0 };
    static constexpr glm::vec2 Right{ 1, 0 };
    static constexpr glm::vec2 Up{ 0, -1 };
    static constexpr glm::vec2 Down{ 0, 1 };
};

using playerId = int;

class PlayerComponent : public dae::BaseComponent
{
public:
    PlayerComponent(dae::GameObject& parent);
    virtual ~PlayerComponent() = default;
    void Update() override;
    void HandleInput(PlayerInputKeys input);

    IdleState& GetIdleState();
    MoveState& GetMoveState();
    AttackState& GetAttackState();
    DieState& GetDieState();

    void Move(glm::vec2 direction);
    bool HasMoved() const;

private:
    void ChangeState(PlayerState* const state);
    void UpdateTimers();

    static constexpr auto MOVED_BUFFER{ 0.1f };

    IdleState m_IdleState{};
    MoveState m_MoveState{};
    AttackState m_AttackState{};
    DieState m_DieState{};

    PlayerState* m_pCurrentState{ &m_IdleState };
    std::unique_ptr<GridMoveCommand> m_MoveCommandLeft{
        std::make_unique<GridMoveCommand>(GetOwner(), PlayerDirection::Left)
    };
    std::unique_ptr<GridMoveCommand> m_MoveCommandRight{
        std::make_unique<GridMoveCommand>(GetOwner(), PlayerDirection::Right)
    };
    std::unique_ptr<GridMoveCommand> m_MoveCommandUp{
        std::make_unique<GridMoveCommand>(GetOwner(), PlayerDirection::Up)
    };
    std::unique_ptr<GridMoveCommand> m_MoveCommandDown{
        std::make_unique<GridMoveCommand>(GetOwner(), PlayerDirection::Down)
    };

    float m_TimeSinceMoved{ 0.0f };
};
