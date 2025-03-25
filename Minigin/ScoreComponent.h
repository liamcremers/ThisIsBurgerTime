#pragma once
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
    class ScoreComponent final : public BaseComponent
    {
    public:
        ScoreComponent(GameObject& parent);
        void AddScore(int addedScore);
        Subject& GetScoreSubject();
        int GetScore() const;

    private:
        Subject m_pScoreSubject;
        int m_Score{};
    };
}
