#pragma once
#include <vector>
#include <BaseComponent.h>

class BurgerGroupComponent : public dae::BaseComponent
{
public:
    BurgerGroupComponent(dae::GameObject& parent);
    void IncrementWalkedOnCounter();
    void EnemyOnBurger();
    void EnemyOffBurger();
    void Reset();
    [[nodiscard]] auto IsFullyWalkedOn() const -> bool;
    [[nodiscard]] auto HasEnemyOnBurger() const -> bool;

private:
    int m_WalkedOnCount{};
    bool m_EnemyOnBurger{};
};
