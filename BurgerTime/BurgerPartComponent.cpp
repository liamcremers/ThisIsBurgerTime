#include "BurgerPartComponent.h"
#include "BurgerTimeLayers.h"
#include "BurgerTimeSoundIds.h"
#include "BurgerGroup.h"
#include "LevelGrid.h"
#include "EnemyComponent.h"

#include <GameObject.h>
#include <BaseComponent.h>
#include <ColliderComponent.h>
#include <PhysicsComponent.h>
#include <ServiceLocator.h>

using namespace BurgerComp;

BurgerPartComponent::BurgerPartComponent(dae::GameObject& parent,
                                         BurgerGroupComponent* burgerGroup) :
    BaseComponent{ parent },
    m_pBurgerGroup{ burgerGroup }
{
    m_pPhysics = GetOwner().GetComponent<dae::PhysicsComponent>();
    m_pCollider = GetOwner().GetComponent<dae::ColliderComponent>();
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

void BurgerComp::BurgerPartComponent::Update()
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

void BurgerComp::BurgerPartComponent::OnEndOverlap(
    const dae::ColliderComponent& other)
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
    const auto& otherSize = other.GetSize();
    const auto& otherPos = other.GetWorldPosition() + otherSize.y / 2.0f;
    const auto& burgerPos = GetOwner().GetWorldPosition();

    return (std::abs(otherPos.x - burgerPos.x) < 6.f &&
            std::abs(otherPos.y - burgerPos.y) < 6.f);
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

bool BurgerComp::BurgerPartComponent::HasAllPartsWalkedOn() const
{
    return m_pBurgerGroup->IsFullyWalkedOn();
}

auto BurgerComp::BurgerPartComponent::HasEnemyOnTop() const -> bool
{
    return m_pBurgerGroup->HasEnemyOnBurger();
}

auto BurgerComp::BurgerPartComponent::GetOverlaps() -> std::set<CollisionLayer>
{
    return m_Overlaps;
}

auto BurgerComp::BurgerPartComponent::GetBeginOverlaps()
    -> std::set<CollisionLayer>
{
    return m_BeginOverlaps;
}

auto BurgerComp::BurgerPartComponent::GetEndOverlaps()
    -> std::set<CollisionLayer>
{
    return m_EndOverlaps;
}

auto BurgerComp::BurgerPartComponent::GetBurgerGroup() -> BurgerGroupComponent*
{
    return m_pBurgerGroup;
}

void BurgerComp::BurgerPartComponent::GoDownOnePixel()
{
    auto& pos = GetOwner().GetWorldPosition();
    GetOwner().SetLocalPosition({ pos.x, pos.y + 1.0f });
}

void BurgerComp::BurgerPartComponent::OnWalkedOn()
{
    m_pBurgerGroup->IncrementWalkedOnCounter();
    GoDownOnePixel();
}

void BurgerComp::BurgerPartComponent::OnIdle()
{
    m_pBurgerGroup->Reset();
    Freeze();
}

void BurgerComp::BurgerPartComponent::OnFalling() { Fall(); }

void BurgerComp::BurgerPartComponent::OnEnemyOnBurgerPart(
    const dae::ColliderComponent& other)
{
    auto EnemyComp =
        other.GetColliderGameObject().GetComponent<EnemyComp::EnemyComponent>();
    (m_pCurrentState == &GetFallingState()) ?
        EnemyComp->OnDieByBurger() :
        EnemyComp->OnMoveWithBurger(GetOwner());
    m_pBurgerGroup->EnemyOnBurger(EnemyComp);
}

void BurgerComp::BurgerPartComponent::OnEnemyOffBurgerPart(
    const dae::ColliderComponent& other)
{
    other.GetColliderGameObject().SetParent(nullptr, true);
    auto EnemyComp =
        other.GetColliderGameObject().GetComponent<EnemyComp::EnemyComponent>();
    m_pBurgerGroup->EnemyOffBurger(EnemyComp);
}

void BurgerComp::BurgerPartComponent::Fall()
{
    m_pPhysics->SetUseGravity(true);
}

void BurgerComp::BurgerPartComponent::Freeze()
{
    m_pPhysics->SetUseGravity(false);
}

void BurgerComp::BurgerPartComponent::ChangeState(BurgerState* newState)
{
    if (m_pCurrentState)
        m_pCurrentState->Exit(*this);
    m_pCurrentState = newState;
    m_pCurrentState->Enter(*this);
}

void BurgerComp::BurgerPartComponent::ResetOverlapChecks()
{
    m_Overlaps.clear();
}
