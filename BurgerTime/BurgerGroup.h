#pragma once
#include <vector>
#include <BaseComponent.h>

class EnemyComponent;

class BurgerGroupComponent : public dae::BaseComponent
{
public:
    explicit BurgerGroupComponent(dae::GameObject& parent);
    void IncrementWalkedOnCounter();
    void EnemyOnBurger(EnemyComponent* pEnemy);
    void EnemyOffBurger(EnemyComponent* pEnemy);
    void Reset();
    [[nodiscard]] auto IsFullyWalkedOn() const -> bool;
    [[nodiscard]] auto HasEnemyOnBurger() const -> bool;

private:
    std::vector<EnemyComponent*> m_pEnemiesOnBurger;
    int m_WalkedOnCount{};
    bool m_HasEnemyOnBurger{};
};
