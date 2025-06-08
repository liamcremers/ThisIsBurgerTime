#pragma once
#include "PlayerState.h"
#include "GridMoveCommand.h"
#include <SpriteComponent.h>
#include <GameObject.h>
#include <BaseComponent.h>

#include <memory>
#include <unordered_map>
#include <glm.hpp>

enum class Direction
{
    Left,
    Right,
    Up,
    Down
};

struct DirectionVec
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
    void CreateOverlapEvent(dae::GameObject& parent);
    void SetupStateTextures();
    virtual ~PlayerComponent() = default;
    void Update() override;
    void HandleInput(PlayerInputKeys input);

    IdleState& GetIdleState();
    MoveState& GetMoveState();
    AttackState& GetAttackState();
    DieState& GetDieState();

    void OnMove(glm::vec2 direction);
    void OnHitByEnemy();
    void OnDeath();
    bool HasMoved() const;

private:
    void UpdateSprite();
    void SetSpriteDirection(glm::vec2 directionVec);
    void LoadStateTexture(PlayerState* stateT,
                          glm::vec2 direction,
                          const std::string& texturePath);
    void ChangeState(PlayerState* const state);
    void UpdateTimers();
    Direction DirectionToEnum(glm::vec2 direction);

    static constexpr auto MOVED_BUFFER{ 0.1f };
    static constexpr float SCALE{ 2.f };
    static constexpr glm::vec2 SPRITE_SIZE{ 16, 16 };
    static constexpr Direction DEFAULT_DIRECTION{ Direction::Down };

    std::unordered_map<PlayerState*, std::unordered_map<Direction, std::string>>
        m_TexturePath;

    IdleState m_IdleState{};
    MoveState m_MoveState{};
    AttackState m_AttackState{};
    DieState m_DieState{};

    PlayerState* m_pCurrentState{ &m_IdleState };
    std::unique_ptr<GridMoveCommand> m_MoveCommandLeft{
        std::make_unique<GridMoveCommand>(GetOwner(), DirectionVec::Left)
    };
    std::unique_ptr<GridMoveCommand> m_MoveCommandRight{
        std::make_unique<GridMoveCommand>(GetOwner(), DirectionVec::Right)
    };
    std::unique_ptr<GridMoveCommand> m_MoveCommandUp{
        std::make_unique<GridMoveCommand>(GetOwner(), DirectionVec::Up)
    };
    std::unique_ptr<GridMoveCommand> m_MoveCommandDown{
        std::make_unique<GridMoveCommand>(GetOwner(), DirectionVec::Down)
    };

    dae::SpriteComponent* m_pSpriteComponent{
        GetOwner().AddComponent<dae::SpriteComponent>(SCALE, SPRITE_SIZE)
    };
    Direction m_Direction{ DEFAULT_DIRECTION };
    float m_TimeSinceMoved{};
};
