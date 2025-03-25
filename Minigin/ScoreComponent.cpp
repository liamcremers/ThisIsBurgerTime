#include "ScoreComponent.h"

dae::ScoreComponent::ScoreComponent(GameObject& parent) :
    BaseComponent{ parent }
{}

void dae::ScoreComponent::AddScore(int addedScore)
{
    m_Score += addedScore;
    m_pScoreSubject.Notify("ScoreUpdated");
}

auto dae::ScoreComponent::GetScore() const -> int { return m_Score; }

auto dae::ScoreComponent::GetScoreSubject() -> dae::Subject&
{
    return m_pScoreSubject;
}