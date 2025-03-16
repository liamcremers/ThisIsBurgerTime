#include "PlayerComponent.h"
#include "InputManager.h"
#include "Controller.h"
#include "Command.h"

#include <SDL.h>

namespace dae
{
    PlayerInputComponent::PlayerInputComponent(GameObject& parent,
                                               unsigned long idx) :
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

        SetUpKeyboardControls(idx);
    }

    PlayerInputComponent::~PlayerInputComponent()
    {
        m_pController->RemoveCommand(*m_pMoveCommandUp, XINPUT_GAMEPAD_DPAD_UP);
        m_pController->RemoveCommand(*m_pMoveCommandDown,
                                     XINPUT_GAMEPAD_DPAD_DOWN);
        m_pController->RemoveCommand(*m_pMoveCommandLeft,
                                     XINPUT_GAMEPAD_DPAD_LEFT);
        m_pController->RemoveCommand(*m_pMoveCommandRight,
                                     XINPUT_GAMEPAD_DPAD_RIGHT);
    }

    auto PlayerInputComponent::GetController() const -> const Controller*
    {
        return m_pController.get();
    }

    void PlayerInputComponent::SetSpeed(int speed)
    {
        m_pMoveCommandUp->SetSpeed(speed);
        m_pMoveCommandDown->SetSpeed(speed);
        m_pMoveCommandLeft->SetSpeed(speed);
        m_pMoveCommandRight->SetSpeed(speed);
    }

    void PlayerInputComponent::SetUpKeyboardControls(unsigned long idx)
    {
        auto& inputManager = InputManager::GetInstance();
        if (idx == 0)
        {
            inputManager.AddKeyboardCommand(SDLK_UP, m_pMoveCommandUp.get());
            inputManager.AddKeyboardCommand(SDLK_DOWN,
                                            m_pMoveCommandDown.get());
            inputManager.AddKeyboardCommand(SDLK_LEFT,
                                            m_pMoveCommandLeft.get());
            inputManager.AddKeyboardCommand(SDLK_RIGHT,
                                            m_pMoveCommandRight.get());
        }
        if (idx == 1)
        {
            inputManager.AddKeyboardCommand(SDLK_w, m_pMoveCommandUp.get());
            inputManager.AddKeyboardCommand(SDLK_s, m_pMoveCommandDown.get());
            inputManager.AddKeyboardCommand(SDLK_a, m_pMoveCommandLeft.get());
            inputManager.AddKeyboardCommand(SDLK_d, m_pMoveCommandRight.get());
        }
    }
}
