#pragma once
#include "EnemyState.h"
#include "GridMoveCommand.h"
#include <SpriteComponent.h>
#include <GameObject.h>
#include <BaseComponent.h>
#include <Observer.h>

#include <memory>
#include <unordered_map>
#include <glm.hpp>

class EnemyComponent : public dae::BaseComponent, public IControllable
{
public:
    explicit EnemyComponent(dae::GameObject& parent);
    virtual ~EnemyComponent() = default;
    void Update() override;
    bool Move(glm::vec2 direction) override;
    void HandleInput(InputKey input) override;

    static dae::Subject& GetStaticDiedSubject();
    EnemyStates::IdleState& GetIdleState();
    EnemyStates::MoveState& GetMoveState();
    EnemyStates::AttackState& GetAttackState();
    EnemyStates::DieState& GetDieState();

    void DieByBurger();
    void MoveWithBurger(dae::GameObject& burger);
    void Die() const;
    bool HasMoved() const;

private:
    void CreateOverlapEvent(dae::GameObject& parent);
    void SetupStateTextures();
    void UpdateSprite();
    void SetSpriteDirection(glm::vec2 directionVec);
    void LoadStateTexture(EnemyStates::EnemyState* stateT,
                          glm::vec2 direction,
                          const std::string& texturePath);
    void ChangeState(EnemyStates::EnemyState* const state);
    void UpdateTimers();
    Direction DirectionToEnum(glm::vec2 direction);

    static constexpr auto MOVED_BUFFER{ 0.1f };
    static constexpr float SCALE{ 2.f };
    static constexpr glm::vec2 SPRITE_SIZE{ 16, 16 };
    static constexpr Direction DEFAULT_DIRECTION{ Direction::Down };

    std::unordered_map<EnemyStates::EnemyState*,
                       std::unordered_map<Direction, std::string>>
        m_TexturePath;

    EnemyStates::IdleState m_IdleState{};
    EnemyStates::MoveState m_MoveState{};
    EnemyStates::AttackState m_AttackState{};
    EnemyStates::DieState m_DieState{};

    EnemyStates::EnemyState* m_pCurrentState{ &m_IdleState };
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
