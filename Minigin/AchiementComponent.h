#pragma once
#include "BaseComponent.h"
#include "ScoreComponent.h"
#include "Observer.h"
#include "steam_api.h"

namespace dae
{
    class AchievementComponent final : public BaseComponent, public Observer
    {
    public:
        AchievementComponent(GameObject& parent, ScoreComponent* pScoreComp);
        ~AchievementComponent();
        void OnNotify(const std::string& eventId) override;
        void OnDestroy() override;

        AchievementComponent(const AchievementComponent&) = delete;
        AchievementComponent(AchievementComponent&&) = delete;
        AchievementComponent& operator=(const AchievementComponent&) = delete;
        AchievementComponent& operator=(AchievementComponent&&) = delete;

    private:
        void UnlockAchievement(const char* achievementID);

        constexpr static int SCORE_TO_UNLOCK = 500;

        ScoreComponent* m_pScoreComponent;
        bool m_Achievement_WinOneGameUnlocked;
    };
}
