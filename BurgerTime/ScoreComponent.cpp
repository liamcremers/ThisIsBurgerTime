#include "ScoreComponent.h"

ScoreComponent::ScoreComponent(dae::GameObject& parent) :
    BaseComponent{ parent }
{}

void ScoreComponent::AddScore(int addedScore)
{
    m_Score += addedScore;
    m_pScoreSubject.Notify("ScoreUpdated");
}

auto ScoreComponent::GetScore() const -> int { return m_Score; }

auto ScoreComponent::GetScoreSubject() -> dae::Subject&
{
    return m_pScoreSubject;
}