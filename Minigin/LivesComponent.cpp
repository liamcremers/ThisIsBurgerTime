#include "LivesComponent.h"
#include <cassert>

dae::LivesComponent::LivesComponent(GameObject& parent, int lives) :
    BaseComponent(parent),
    m_Lives(lives)
{}

void dae::LivesComponent::LoseLife()
{
    if (m_Lives < 0)
        assert("Dead player cannot lose more lives" && false);

    --m_Lives;
    m_LivesSubject.Notify(m_Lives < 0 ? "GameOver" : "LifeLost");
    //Notify(m_Lives < 0 ? "GameOver" : "LifeLost");
}

auto dae::LivesComponent::GetLives() const -> int { return m_Lives; }

auto dae::LivesComponent::GetLivesSubject() -> dae::Subject
{
    return m_LivesSubject;
}
