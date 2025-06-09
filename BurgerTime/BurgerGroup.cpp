#include "BurgerGroup.h"

static constexpr int MAX_WALKED_ON_COUNT = 4;

BurgerGroupComponent::BurgerGroupComponent(dae::GameObject& parent) :
    BaseComponent(parent)
{}

void BurgerGroupComponent::IncrementWalkedOnCounter() { ++m_WalkedOnCount; }

void BurgerGroupComponent::EnemyOnBurger() { m_EnemyOnBurger = true; }

void BurgerGroupComponent::EnemyOffBurger() { m_EnemyOnBurger = false; }

void BurgerGroupComponent::Reset() { m_WalkedOnCount = 0; }

[[nodiscard]]
auto BurgerGroupComponent::IsFullyWalkedOn() const -> bool
{
    return m_WalkedOnCount >= MAX_WALKED_ON_COUNT;
}

[[nodiscard]]
auto BurgerGroupComponent::HasEnemyOnBurger() const -> bool
{
    return m_EnemyOnBurger;
}
