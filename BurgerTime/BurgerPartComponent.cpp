#include "BurgerPartComponent.h"
#include "BurgerTimeLayers.h"
#include "BurgerTimeSoundIds.h"
#include "BurgerGroup.h"
#include "LevelGrid.h"
#include "EnemyComponent.h"

#include <ColliderComponent.h>
#include <GameObject.h>
#include <BaseComponent.h>
#include <ServiceLocator.h>

using namespace BurgerComp;

BurgerPartComponent::BurgerPartComponent(dae::GameObject& parent,
                                         BurgerGroupComponent* burgerGroup) :
    BaseComponent{ parent },
    m_pPhysics{ GetOwner().GetComponent<dae::PhysicsComponent>() },
    m_pCollider{ GetOwner().GetComponent<dae::ColliderComponent>() },
    m_pBurgerGroup{ burgerGroup }
{
    assert((m_pPhysics && m_pCollider) &&
           "BurgerPartComponent requires a PhysicsComponent and a "
           "ColliderComponent");

    m_pCollider->SetCollisionType(CollisionType::Trigger);
    m_pCollider->SubscribeToOverlap([this](const dae::ColliderComponent& other)
                                    { OnOverlap(other); });
    m_pCollider->SubscribeToBeginOverlap(
        [this](const dae::ColliderComponent& other) { OnBeginOverlap(other); });
    m_pCollider->SubscribeToEndOverlap(
        [this](const dae::ColliderComponent& other) { OnEndOverlap(other); });
}

void BurgerPartComponent::Update()
{
    auto& newState = m_pCurrentState->Update(*this);
    if (&newState != m_pCurrentState)
    {
        ChangeState(&newState);
    }
    m_Overlaps.clear();
    m_BeginOverlaps.clear();
    m_EndOverlaps.clear();
}

void BurgerPartComponent::OnOverlap(const dae::ColliderComponent& other)
{
    if (IsLayerType(other, CollisionLayer::Player) && IsAligned(other))
        m_Overlaps.insert(CollisionLayer::Player);
}

void BurgerPartComponent::OnBeginOverlap(const dae::ColliderComponent& other)
{
    if (IsLayerType(other, CollisionLayer::Floor) or
        IsLayerType(other, CollisionLayer::BurgerPlate))
    {
        m_BeginOverlaps.insert(static_cast<CollisionLayer>(other.GetLayer()));
    }

    if (IsLayerType(other, CollisionLayer::Enemy))
        OnEnemyOnBurgerPart(other);
}

void BurgerPartComponent::OnEndOverlap(const dae::ColliderComponent& other)
{
    if (IsLayerType(other, CollisionLayer::Floor) or
        IsLayerType(other, CollisionLayer::BurgerPlate))
    {
        m_EndOverlaps.insert(static_cast<CollisionLayer>(other.GetLayer()));
    }

    if (IsLayerType(other, CollisionLayer::Enemy))
        OnEnemyOffBurgerPart(other);
}

auto BurgerPartComponent::IsAligned(const dae::ColliderComponent& other) -> bool
{
    static constexpr float ALIGNED_DISTANCE = 6.f;
    static constexpr float HALF = 2.0f;
    const auto& otherSize = other.GetSize();
    const auto& otherPos = other.GetWorldPosition() + otherSize[1] / HALF;
    const auto& burgerPos = GetOwner().GetWorldPosition();

    return (std::abs(otherPos[0] - burgerPos[0]) < ALIGNED_DISTANCE &&
            std::abs(otherPos[1] - burgerPos[1]) < ALIGNED_DISTANCE);
}

auto BurgerPartComponent::GetIdleState() -> IdleState& { return m_IdleState; }

auto BurgerPartComponent::GetWalkedOnState() -> WalkedOnState&
{
    return m_WalkedOnState;
}

auto BurgerPartComponent::GetFallingState() -> FallingState&
{
    return m_FallingState;
}

auto BurgerPartComponent::HasAllPartsWalkedOn() const -> bool
{
    return m_pBurgerGroup->IsFullyWalkedOn();
}

auto BurgerPartComponent::HasEnemyOnTop() const -> bool
{
    return m_pBurgerGroup->HasEnemyOnBurger();
}

auto BurgerPartComponent::GetOverlaps() -> std::set<CollisionLayer>
{
    return m_Overlaps;
}

auto BurgerPartComponent::GetBeginOverlaps() -> std::set<CollisionLayer>
{
    return m_BeginOverlaps;
}

auto BurgerPartComponent::GetEndOverlaps() -> std::set<CollisionLayer>
{
    return m_EndOverlaps;
}

auto BurgerPartComponent::GetBurgerGroup() -> BurgerGroupComponent*
{
    return m_pBurgerGroup;
}

void BurgerPartComponent::GoDownOnePixel()
{
    const auto& pos = GetOwner().GetWorldPosition();
    GetOwner().SetLocalPosition({ pos[0], (pos[1] + 1.0f) });
}

void BurgerPartComponent::OnWalkedOn()
{
    m_pBurgerGroup->IncrementWalkedOnCounter();
    GoDownOnePixel();
}

void BurgerPartComponent::OnIdle()
{
    m_pBurgerGroup->Reset();
    Freeze();
}

void BurgerPartComponent::OnFalling() { Fall(); }

void BurgerPartComponent::OnEnemyOnBurgerPart(
    const dae::ColliderComponent& other)
{
    auto* EnemyComp =
        other.GetColliderGameObject().GetComponent<EnemyComponent>();
    (m_pCurrentState == &GetFallingState()) ?
        EnemyComp->OnDieByBurger() :
        EnemyComp->OnMoveWithBurger(GetOwner());
    m_pBurgerGroup->EnemyOnBurger(EnemyComp);
}

void BurgerPartComponent::OnEnemyOffBurgerPart(
    const dae::ColliderComponent& other)
{
    other.GetColliderGameObject().SetParent(nullptr, true);
    auto* EnemyComp =
        other.GetColliderGameObject().GetComponent<EnemyComponent>();
    m_pBurgerGroup->EnemyOffBurger(EnemyComp);
}

void BurgerPartComponent::Fall() { m_pPhysics->SetUseGravity(true); }

void BurgerPartComponent::Freeze() { m_pPhysics->SetUseGravity(false); }

void BurgerPartComponent::ChangeState(BurgerState* newState)
{
    if (m_pCurrentState)
        m_pCurrentState->Exit(*this);
    m_pCurrentState = newState;
    m_pCurrentState->Enter(*this);
}

void BurgerPartComponent::ResetOverlapChecks() { m_Overlaps.clear(); }
