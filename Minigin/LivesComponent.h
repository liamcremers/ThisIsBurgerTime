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
        Subject& GetLivesSubject();
        int GetLives() const;

    private:
        Subject m_LivesSubject{};
        int m_Lives{};
    };
}
