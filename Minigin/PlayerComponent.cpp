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
        m_pMoveCommandRight{ std::make_unique<MoveCommand>(parent, MoveRight) },
        m_pSelfDamageCommand{ std::make_unique<SelfDamageCommand>(parent) },
        m_pAdd100PointsCommand{
            std::make_unique<AddPointsCommand>(parent, HUNDRED_POINTS)
        },
        m_pAdd10PointsCommand{ std::make_unique<AddPointsCommand>(parent,
                                                                  TEN_POINTS) }

    {
        InputManager::GetInstance().AddController(m_pController.get());
        m_pController->AddCommand(*m_pMoveCommandUp,
                                  XINPUT_GAMEPAD_DPAD_UP,
                                  dae::ButtonState::Pressed);
        m_pController->AddCommand(*m_pMoveCommandDown,
                                  XINPUT_GAMEPAD_DPAD_DOWN,
                                  dae::ButtonState::Pressed);
        m_pController->AddCommand(*m_pMoveCommandLeft,
                                  XINPUT_GAMEPAD_DPAD_LEFT,
                                  dae::ButtonState::Pressed);
        m_pController->AddCommand(*m_pMoveCommandRight,
                                  XINPUT_GAMEPAD_DPAD_RIGHT,
                                  dae::ButtonState::Pressed);
        m_pController->AddCommand(*m_pSelfDamageCommand,
                                  XINPUT_GAMEPAD_X,
                                  dae::ButtonState::DownThisFrame);
        m_pController->AddCommand(*m_pAdd100PointsCommand,
                                  XINPUT_GAMEPAD_A,
                                  dae::ButtonState::DownThisFrame);
        m_pController->AddCommand(*m_pAdd10PointsCommand,
                                  XINPUT_GAMEPAD_B,
                                  dae::ButtonState::DownThisFrame);

        SetUpKeyboardControls(idx);
    }

    PlayerInputComponent::~PlayerInputComponent()
    {
        m_pController->RemoveCommand(*m_pMoveCommandUp,
                                     XINPUT_GAMEPAD_DPAD_UP,
                                     dae::ButtonState::Pressed);
        m_pController->RemoveCommand(*m_pMoveCommandDown,
                                     XINPUT_GAMEPAD_DPAD_DOWN,
                                     dae::ButtonState::Pressed);
        m_pController->RemoveCommand(*m_pMoveCommandLeft,
                                     XINPUT_GAMEPAD_DPAD_LEFT,
                                     dae::ButtonState::Pressed);
        m_pController->RemoveCommand(*m_pMoveCommandRight,
                                     XINPUT_GAMEPAD_DPAD_RIGHT,
                                     dae::ButtonState::Pressed);
        m_pController->RemoveCommand(*m_pSelfDamageCommand,
                                     XINPUT_GAMEPAD_X,
                                     dae::ButtonState::DownThisFrame);
        m_pController->RemoveCommand(*m_pAdd100PointsCommand,
                                     XINPUT_GAMEPAD_A,
                                     dae::ButtonState::DownThisFrame);
        m_pController->RemoveCommand(*m_pAdd10PointsCommand,
                                     XINPUT_GAMEPAD_B,
                                     dae::ButtonState::DownThisFrame);
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
            inputManager.AddKeyboardCommand(SDLK_c, m_pSelfDamageCommand.get());
            inputManager.AddKeyboardCommand(SDLK_z,
                                            m_pAdd100PointsCommand.get());
            inputManager.AddKeyboardCommand(SDLK_q,
                                            m_pAdd10PointsCommand.get());
        }
    }
}
