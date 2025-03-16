#include "AchiementComponent.h"
#include "GameObject.h"
#include <iostream>

dae::AchievementComponent::AchievementComponent(GameObject& parent,
                                                ScoreComponent* pScoreComp) :
    BaseComponent(parent),
    m_pScoreComponent(pScoreComp),
    m_AchievementUnlocked(false)
{
    assert(m_pScoreComponent &&
           "dae::AchievementComponent depends on ScoreComponent");
    m_pScoreComponent->AddObserver(this);

    if (!SteamAPI_Init())
    {
        std::cerr << "Steam must be running to unlock achievements.\n";
    }
}

void dae::AchievementComponent::OnNotify(const std::string& eventId)
{
    if (eventId == "ScoreUpdated" && !m_AchievementUnlocked)
    {
        if (m_pScoreComponent->GetScore() >= 500)
        {
            UnlockAchievement("ACH_WIN_ONE_GAME");
            m_AchievementUnlocked = true;
        }
    }
}

void dae::AchievementComponent::UnlockAchievement(const char* achievementID)
{
    if (SteamUserStats())
    {
        SteamUserStats()->SetAchievement(achievementID);
        SteamUserStats()->StoreStats();
        std::cout << "Achievement unlocked: " << achievementID << "\n";
    }
}
