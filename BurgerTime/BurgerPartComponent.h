#pragma once
#include "BurgerState.h"
#include <BaseComponent.h>
#include <set>

namespace dae
{
    class PhysicsComponent;
    class ColliderComponent;
}
class BurgerGroupComponent;
enum class CollisionLayer;

namespace BurgerComp
{
    class BurgerPartComponent final : public dae::BaseComponent
    {
        using IdleState = BurgerStates::IdleState;
        using WalkedOnState = BurgerStates::WalkedOnState;
        using FallingState = BurgerStates::FallingState;
        using BurgerState = BurgerStates::BurgerState;

    public:
        BurgerPartComponent(dae::GameObject& parent,
                            BurgerGroupComponent* burgerGroup);
        virtual ~BurgerPartComponent() = default;
        void Update() override;

        IdleState& GetIdleState();
        WalkedOnState& GetWalkedOnState();
        FallingState& GetFallingState();

        bool HasAllPartsWalkedOn() const;
        auto HasEnemyOnTop() const -> bool;

        auto GetOverlaps() -> std::set<CollisionLayer>;
        auto GetBeginOverlaps() -> std::set<CollisionLayer>;
        auto GetEndOverlaps() -> std::set<CollisionLayer>;
        auto GetBurgerGroup() -> BurgerGroupComponent*;

        void OnWalkedOn();
        void OnIdle();
        void OnFalling();
        void OnEnemyOnBurgerPart(const dae::ColliderComponent& other);
        void OnEnemyOffBurgerPart(const dae::ColliderComponent& other);

    private:
        void ChangeState(BurgerState* newState);
        bool IsAligned(const dae::ColliderComponent& other);
        void GoDownOnePixel();
        void Freeze();
        void Fall();

        void ResetOverlapChecks();
        void OnOverlap(const dae::ColliderComponent& other);
        void OnBeginOverlap(const dae::ColliderComponent& other);
        void OnEndOverlap(const dae::ColliderComponent& other);

        std::set<CollisionLayer> m_Overlaps{};
        std::set<CollisionLayer> m_BeginOverlaps{};
        std::set<CollisionLayer> m_EndOverlaps{};
        bool m_HasEnemyOnTop{};

        IdleState m_IdleState{};
        WalkedOnState m_WalkedOnState{};
        FallingState m_FallingState{};
        BurgerState* m_pCurrentState{ &m_IdleState };

        dae::PhysicsComponent* m_pPhysics{};
        dae::ColliderComponent* m_pCollider{};
        BurgerGroupComponent* m_pBurgerGroup{};
    };
}
