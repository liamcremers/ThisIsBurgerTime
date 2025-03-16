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
        ~AchievementComponent() override = default;

        AchievementComponent(const AchievementComponent&) = delete;
        AchievementComponent(AchievementComponent&&) = delete;
        AchievementComponent& operator=(const AchievementComponent&) = delete;
        AchievementComponent& operator=(AchievementComponent&&) = delete;

    protected:
        void OnNotify(const std::string& eventId) override;
        void OnDestroy() override;

    private:
        constexpr static int SCORE_TO_UNLOCK = 500;

        ScoreComponent* m_pScoreComponent;
        bool m_AchievementUnlocked;

        void UnlockAchievement(const char* achievementID);
    };
}
