#include "LivesComponent.h"
#include <cassert>

LivesComponent::LivesComponent(dae::GameObject& parent, int lives) :
    BaseComponent(parent),
    m_Lives(lives)
{}

void LivesComponent::LoseLife()
{
    if (m_Lives < 0)
        assert("Dead player cannot lose more lives" && false);

    --m_Lives;
    m_LivesSubject.Notify(m_Lives < 0 ? "GameOver" : "LifeLost");
}

auto LivesComponent::GetLives() const -> int { return m_Lives; }

auto LivesComponent::GetLivesSubject() -> dae::Subject&
{
    return m_LivesSubject;
}
