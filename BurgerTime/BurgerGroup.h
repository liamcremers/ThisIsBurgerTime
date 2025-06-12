#pragma once
#include <vector>
#include <BaseComponent.h>

namespace EnemyComp
{
    class EnemyComponent;
}

class BurgerGroupComponent : public dae::BaseComponent
{
public:
    BurgerGroupComponent(dae::GameObject& parent);
    void IncrementWalkedOnCounter();
    void EnemyOnBurger(EnemyComp::EnemyComponent* pEnemy);
    void EnemyOffBurger(EnemyComp::EnemyComponent* pEnemy);
    void Reset();
    [[nodiscard]] auto IsFullyWalkedOn() const -> bool;
    [[nodiscard]] auto HasEnemyOnBurger() const -> bool;

private:
    std::vector<EnemyComp::EnemyComponent*> m_pEnemiesOnBurger;
    int m_WalkedOnCount{};
    bool m_HasEnemyOnBurger{};
};
