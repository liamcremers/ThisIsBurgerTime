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

namespace EnemyComp
{
    enum class Direction
    {
        Left,
        Right,
        Up,
        Down,
        None
    };

    struct DirectionVec
    {
        static constexpr glm::vec2 Left{ -1, 0 };
        static constexpr glm::vec2 Right{ 1, 0 };
        static constexpr glm::vec2 Up{ 0, -1 };
        static constexpr glm::vec2 Down{ 0, 1 };
        static constexpr glm::vec2 None{ 0, 0 };
    };

    using EnemyId = int;
    using IdleState = EnemyStates::IdleState;
    using MoveState = EnemyStates::MoveState;
    using AttackState = EnemyStates::AttackState;
    using DieState = EnemyStates::DieState;
    using EnemyState = EnemyStates::EnemyState;

    class EnemyComponent : public dae::BaseComponent
    {
    public:
        EnemyComponent(dae::GameObject& parent);
        virtual ~EnemyComponent() = default;
        void Update() override;
        void HandleInput(EnemyInputKeys input);

        static dae::Subject& GetStaticDiedSubject();
        IdleState& GetIdleState();
        MoveState& GetMoveState();
        AttackState& GetAttackState();
        DieState& GetDieState();

        void OnMove(glm::vec2 direction);
        void OnDieByBurger();
        void OnMoveWithBurger(dae::GameObject& burger);
        void OnDeath();
        bool HasMoved() const;

    private:
        void CreateOverlapEvent(dae::GameObject& parent);
        void SetupStateTextures();
        void UpdateSprite();
        void SetSpriteDirection(glm::vec2 directionVec);
        void LoadStateTexture(EnemyState* stateT,
                              glm::vec2 direction,
                              const std::string& texturePath);
        void ChangeState(EnemyState* const state);
        void UpdateTimers();
        Direction DirectionToEnum(glm::vec2 direction);

        static constexpr auto MOVED_BUFFER{ 0.1f };
        static constexpr float SCALE{ 2.f };
        static constexpr glm::vec2 SPRITE_SIZE{ 16, 16 };
        static constexpr Direction DEFAULT_DIRECTION{ Direction::Down };

        std::unordered_map<EnemyState*,
                           std::unordered_map<Direction, std::string>>
            m_TexturePath;

        IdleState m_IdleState{};
        MoveState m_MoveState{};
        AttackState m_AttackState{};
        DieState m_DieState{};

        EnemyState* m_pCurrentState{ &m_IdleState };
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
}
