#pragma once
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
    class LivesComponent final : public BaseComponent
    {
    public:
        LivesComponent(GameObject& parent, int lives);
        void LoseLife();
        [[nodiscard]] auto GetLivesSubject() -> Subject&;
        [[nodiscard]] auto GetLives() const -> int;

    private:
        Subject m_LivesSubject{};
        int m_Lives{};
    };
}
