#pragma once
#include "BaseComponent.h"
#include "Observer.h"

class ScoreComponent final : public dae::BaseComponent
{
public:
    explicit ScoreComponent(dae::GameObject& parent);
    void AddScore(int addedScore);
    [[nodiscard]] auto GetScoreSubject() -> dae::Subject&;
    [[nodiscard]] auto GetScore() const -> int;

private:
    dae::Subject m_pScoreSubject;
    int m_Score{};
};
