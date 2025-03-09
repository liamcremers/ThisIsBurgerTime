#include "PlayerComponent.h"
#include "InputManager.h"
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
    {
        InputManager::GetInstance().AddController(m_pController.get());
        m_pController->AddCommand(*m_pMoveCommandUp, XINPUT_GAMEPAD_DPAD_UP);
        m_pController->AddCommand(*m_pMoveCommandDown,
                                  XINPUT_GAMEPAD_DPAD_DOWN);
        m_pController->AddCommand(*m_pMoveCommandLeft,
                                  XINPUT_GAMEPAD_DPAD_LEFT);
        m_pController->AddCommand(*m_pMoveCommandRight,
                                  XINPUT_GAMEPAD_DPAD_RIGHT);
    }

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
