#pragma once
#include "BaseComponent.h"
#include "Observer.h"

class LivesComponent final : public dae::BaseComponent
{
public:
    explicit LivesComponent(dae::GameObject& parent, int lives);
    void LoseLife();
    [[nodiscard]] auto GetLivesSubject() -> dae::Subject&;
    [[nodiscard]] auto GetLives() const -> int;

private:
    dae::Subject m_LivesSubject{};
    int m_Lives{};
};
