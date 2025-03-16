#pragma once
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
    class LivesComponent final : public BaseComponent, public Subject
    {
    public:
        LivesComponent(GameObject& parent, int lives);
        void LoseLife();
        int GetLives() const;

    private:
        int m_Lives{};
        int m_MaxHealth{};
    };
}
