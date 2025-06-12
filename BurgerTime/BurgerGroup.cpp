#include "BurgerGroup.h"
#include "EnemyComponent.h"
#include <algorithm>
#include <ranges>

static constexpr int MAX_WALKED_ON_COUNT = 4;

BurgerGroupComponent::BurgerGroupComponent(dae::GameObject& parent) :
    BaseComponent(parent)
{}

void BurgerGroupComponent::IncrementWalkedOnCounter() { ++m_WalkedOnCount; }

void BurgerGroupComponent::EnemyOnBurger(EnemyComp::EnemyComponent* pEnemy)
{
    m_HasEnemyOnBurger = true;
    m_pEnemiesOnBurger.emplace_back(pEnemy);
}

void BurgerGroupComponent::EnemyOffBurger(EnemyComp::EnemyComponent* pEnemy)
{
    std::erase_if(m_pEnemiesOnBurger,
                  [pEnemy](const auto& enemy) { return enemy == pEnemy; });
    m_HasEnemyOnBurger = false;
}

void BurgerGroupComponent::Reset()
{
    m_WalkedOnCount = 0;
    std::ranges::for_each(m_pEnemiesOnBurger, &EnemyComponent::OnDieByBurger);
}

[[nodiscard]]
auto BurgerGroupComponent::IsFullyWalkedOn() const -> bool
{
    return m_WalkedOnCount >= MAX_WALKED_ON_COUNT;
}

[[nodiscard]]
auto BurgerGroupComponent::HasEnemyOnBurger() const -> bool
{
    return m_HasEnemyOnBurger;
}
