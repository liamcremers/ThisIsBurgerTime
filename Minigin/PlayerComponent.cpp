#include "PlayerComponent.h"
#include "Controller.h"
#include "Command.h"

namespace dae
{
    PlayerComponent::PlayerComponent(GameObject& parent, unsigned long idx) :
        BaseComponent(parent),
        m_pController{ std::make_unique<Controller>(idx) },
        m_pMoveCommandUp{ std::make_unique<MoveCommand>(parent, MoveUp) },
        m_pMoveCommandDown{ std::make_unique<MoveCommand>(parent, MoveDown) },
        m_pMoveCommandLeft{ std::make_unique<MoveCommand>(parent, MoveLeft) },
        m_pMoveCommandRight{ std::make_unique<MoveCommand>(parent, MoveRight) }
    {}

    PlayerComponent::~PlayerComponent() = default;

    const Controller* PlayerComponent::GetController() const
    {
        return m_pController.get();
    }

    void PlayerComponent::SetSpeed(int speed)
    {
        m_pMoveCommandUp->SetSpeed(speed);
        m_pMoveCommandDown->SetSpeed(speed);
        m_pMoveCommandLeft->SetSpeed(speed);
        m_pMoveCommandRight->SetSpeed(speed);
    }
}
