#pragma once
#include "PlayerState.h"
#include "GridMoveCommand.h"
#include "IControllable.h"

#include <SpriteComponent.h>
#include <GameObject.h>
#include <BaseComponent.h>
#include <Observer.h>

#include <memory>
#include <unordered_map>
#include <glm.hpp>

class PlayerComponent :
    public dae::BaseComponent,
    public dae::Observer,
    public IControllable
{
public:
    explicit PlayerComponent(dae::GameObject& parent);
    virtual ~PlayerComponent();
    void Update() override;
    void HandleInput(InputKey input) override;

    PlayerStates::IdleState& GetIdleState();
    PlayerStates::MoveState& GetMoveState();
    PlayerStates::AttackState& GetAttackState();
    PlayerStates::DieState& GetDieState();

    void OnHitByEnemy();
    void Die();
    bool HasMoved() const;
    auto GetPosition() const -> glm::vec2;
    bool Move(glm::vec2 direction);

private:
    void CreateOverlapEvent(dae::GameObject& parent);
    void SetupStateTextures();
    void UpdateSprite();
    void SetSpriteDirection(glm::vec2 directionVec);
    void LoadStateTexture(PlayerStates::PlayerState* stateT,
                          glm::vec2 direction,
                          const std::string& texturePath);
    void ChangeState(PlayerStates::PlayerState* const state);
    void UpdateTimers();
    bool IsAlignedWithEnemy(const dae::ColliderComponent& other);
    Direction DirectionToEnum(glm::vec2 direction);

    static constexpr auto MOVED_BUFFER{ 0.1f };
    static constexpr float SCALE{ 2.f };
    static constexpr glm::vec2 SPRITE_SIZE{ 16, 16 };
    static constexpr Direction DEFAULT_DIRECTION{ Direction::Down };

    std::unordered_map<PlayerStates::PlayerState*,
                       std::unordered_map<Direction, std::string>>
        m_TexturePath;

    PlayerStates::IdleState m_IdleState{};
    PlayerStates::MoveState m_MoveState{};
    PlayerStates::AttackState m_AttackState{};
    PlayerStates::DieState m_DieState{};

    PlayerStates::PlayerState* m_pCurrentState{ &m_IdleState };
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

protected:
    void OnNotify(const std::string& eventId,
                  const std::any& args = std::any{}) override;

    void OnDestroy() override {}
};