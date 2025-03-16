#pragma once
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
    class ScoreComponent final : public BaseComponent, public Subject
    {
    public:
        ScoreComponent(GameObject& parent);
        void AddScore(int addedScore);
        int GetScore() const;

    private:
        int m_Score{};
    };
}
