#include "PlayerInputComponent.h"
#include "GridMoveCommand.h"

#include <InputManager.h>
#include <Controller.h>
#include <GameObject.h>
#include <ScoreComponent.h>
#include <LivesComponent.h>

#include <SDL.h>

PlayerInputComponent::PlayerInputComponent(dae::GameObject& parent,
                                           unsigned long idx) :
    BaseComponent(parent),
    m_pController{ std::make_unique<dae::Controller>(idx) },
    m_pMoveCommandUp{ std::make_unique<GridMoveCommand>(parent, MoveUp) },
    m_pMoveCommandDown{ std::make_unique<GridMoveCommand>(parent, MoveDown) },
    m_pMoveCommandLeft{ std::make_unique<GridMoveCommand>(parent, MoveLeft) },
    m_pMoveCommandRight{ std::make_unique<GridMoveCommand>(parent, MoveRight) }
{
    dae::InputManager::GetInstance().AddController(m_pController.get());
    m_pController->AddCommand(
        *m_pMoveCommandUp, XINPUT_GAMEPAD_DPAD_UP, dae::ButtonState::Pressed);
    m_pController->AddCommand(*m_pMoveCommandDown,
                              XINPUT_GAMEPAD_DPAD_DOWN,
                              dae::ButtonState::Pressed);
    m_pController->AddCommand(*m_pMoveCommandLeft,
                              XINPUT_GAMEPAD_DPAD_LEFT,
                              dae::ButtonState::Pressed);
    m_pController->AddCommand(*m_pMoveCommandRight,
                              XINPUT_GAMEPAD_DPAD_RIGHT,
                              dae::ButtonState::Pressed);
    SetUpKeyboardControls(idx);
}

PlayerInputComponent::~PlayerInputComponent()
{
    m_pController->RemoveCommand(
        *m_pMoveCommandUp, XINPUT_GAMEPAD_DPAD_UP, dae::ButtonState::Pressed);
    m_pController->RemoveCommand(*m_pMoveCommandDown,
                                 XINPUT_GAMEPAD_DPAD_DOWN,
                                 dae::ButtonState::Pressed);
    m_pController->RemoveCommand(*m_pMoveCommandLeft,
                                 XINPUT_GAMEPAD_DPAD_LEFT,
                                 dae::ButtonState::Pressed);
    m_pController->RemoveCommand(*m_pMoveCommandRight,
                                 XINPUT_GAMEPAD_DPAD_RIGHT,
                                 dae::ButtonState::Pressed);
}

auto PlayerInputComponent::GetController() const -> const dae::Controller*
{
    return m_pController.get();
}

void PlayerInputComponent::SetCanMove(bool canMove)
{
    m_pMoveCommandUp->SetCanMove(canMove);
    m_pMoveCommandDown->SetCanMove(canMove);
    m_pMoveCommandLeft->SetCanMove(canMove);
    m_pMoveCommandRight->SetCanMove(canMove);
}

GridMoveCommand* PlayerInputComponent::GetMoveCommandUp() const
{
    return m_pMoveCommandUp.get();
}

GridMoveCommand* PlayerInputComponent::GetMoveCommandDown() const
{
    return m_pMoveCommandDown.get();
}

GridMoveCommand* PlayerInputComponent::GetMoveCommandLeft() const
{
    return m_pMoveCommandLeft.get();
}

GridMoveCommand* PlayerInputComponent::GetMoveCommandRight() const
{
    return m_pMoveCommandRight.get();
}

void PlayerInputComponent::SetUpKeyboardControls(unsigned long idx)
{
    auto& inputManager = dae::InputManager::GetInstance();
    if (idx == 0)
    {
        inputManager.AddKeyboardCommand(SDLK_UP, m_pMoveCommandUp.get());
        inputManager.AddKeyboardCommand(SDLK_DOWN, m_pMoveCommandDown.get());
        inputManager.AddKeyboardCommand(SDLK_LEFT, m_pMoveCommandLeft.get());
        inputManager.AddKeyboardCommand(SDLK_RIGHT, m_pMoveCommandRight.get());
    }
    if (idx == 1)
    {
        inputManager.AddKeyboardCommand(SDLK_w, m_pMoveCommandUp.get());
        inputManager.AddKeyboardCommand(SDLK_s, m_pMoveCommandDown.get());
        inputManager.AddKeyboardCommand(SDLK_a, m_pMoveCommandLeft.get());
        inputManager.AddKeyboardCommand(SDLK_d, m_pMoveCommandRight.get());
    }
}
