#include "AchiementComponent.h"
#include "GameObject.h"
#include <iostream>

dae::AchievementComponent::AchievementComponent(GameObject& parent,
                                                ScoreComponent* pScoreComp) :
    BaseComponent(parent),
    m_pScoreComponent(pScoreComp),
    m_Achievement_WinOneGameUnlocked(false)
{
    assert(m_pScoreComponent &&
           "dae::AchievementComponent depends on ScoreComponent");
    m_pScoreComponent->GetScoreSubject().AddObserver(this);

    if (!SteamAPI_Init())
    {
        assert("Steam must be running to unlock achievements.\n" && false);
    }
}

dae::AchievementComponent::~AchievementComponent()
{
    if (m_pScoreComponent)
        m_pScoreComponent->GetScoreSubject().RemoveObserver(this);
}

void dae::AchievementComponent::OnNotify(const std::string& eventId)
{
    if (eventId == "ScoreUpdated" && !m_Achievement_WinOneGameUnlocked)
    {
        if (m_pScoreComponent->GetScore() >= SCORE_TO_UNLOCK)
        {
            UnlockAchievement("ACH_WIN_ONE_GAME");
            m_Achievement_WinOneGameUnlocked = true;
        }
    }
}

void dae::AchievementComponent::OnDestroy() { m_pScoreComponent = nullptr; }

void dae::AchievementComponent::UnlockAchievement(const char* achievementID)
{
    if (SteamUserStats())
    {
        SteamUserStats()->SetAchievement(achievementID);
        SteamUserStats()->StoreStats();
    }
}
