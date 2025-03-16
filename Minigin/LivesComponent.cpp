#include "LivesComponent.h"
#include <cassert>

dae::LivesComponent::LivesComponent(GameObject& parent, int lives) :
    BaseComponent(parent),
    m_Lives(lives)
{}

void dae::LivesComponent::LoseLife()
{
    if (m_Lives < 0)
        assert("GameObject already has this component" && false);

    --m_Lives;
    Notify(m_Lives < 0 ? "GameOver" : "LifeLost");
}

auto dae::LivesComponent::GetLives() const -> int { return m_Lives; }