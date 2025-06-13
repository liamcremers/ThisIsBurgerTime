#include "InputComponent.h"
#include <InputManager.h>
#include <Controller.h>
#include <GameObject.h>

#include <SDL.h>

InputComponent::InputComponent(dae::GameObject& parent,
                               unsigned long idx,
                               IControllable* controllable) :
    dae::BaseComponent{ parent },
    m_pControllable{ controllable },
    m_pController{ std::make_unique<dae::Controller>(idx) }
{
    m_Commands[MOVE_UP] =
        std::make_unique<InputCommand>(parent, MOVE_UP, m_pControllable);
    m_Commands[MOVE_DOWN] =
        std::make_unique<InputCommand>(parent, MOVE_DOWN, m_pControllable);
    m_Commands[MOVE_LEFT] =
        std::make_unique<InputCommand>(parent, MOVE_LEFT, m_pControllable);
    m_Commands[MOVE_RIGHT] =
        std::make_unique<InputCommand>(parent, MOVE_RIGHT, m_pControllable);
    m_Commands[ATTACK] =
        std::make_unique<InputCommand>(parent, ATTACK, m_pControllable);

    dae::InputManager::GetInstance().AddController(m_pController.get());
    m_pController->AddCommand(*m_Commands[MOVE_UP], GAMEPAD_DPAD_U, PRESSED);
    m_pController->AddCommand(*m_Commands[MOVE_DOWN], GAMEPAD_DPAD_D, PRESSED);
    m_pController->AddCommand(*m_Commands[MOVE_LEFT], GAMEPAD_DPAD_L, PRESSED);
    m_pController->AddCommand(*m_Commands[MOVE_RIGHT], GAMEPAD_DPAD_R, PRESSED);
    m_pController->AddCommand(*m_Commands[ATTACK], GAMEPAD_A, PRESSED);

    SetUpKeyboardControls(idx);
}

InputComponent::~InputComponent()
{
    m_pController->RemoveCommand(*m_Commands[MOVE_UP], GAMEPAD_DPAD_U, PRESSED);
    m_pController->RemoveCommand(
        *m_Commands[MOVE_DOWN], GAMEPAD_DPAD_D, PRESSED);
    m_pController->RemoveCommand(
        *m_Commands[MOVE_LEFT], GAMEPAD_DPAD_L, PRESSED);
    m_pController->RemoveCommand(
        *m_Commands[MOVE_RIGHT], GAMEPAD_DPAD_R, PRESSED);
    m_pController->RemoveCommand(*m_Commands[ATTACK], GAMEPAD_A, PRESSED);
}

auto InputComponent::GetController() const -> const dae::Controller*
{
    return m_pController.get();
}

void InputComponent::SetUpKeyboardControls(unsigned long idx)
{
    auto& inputMgr = dae::InputManager::GetInstance();

    if (idx == 0)
    {
        inputMgr.AddKeyboardCommand(SDLK_UP, m_Commands[MOVE_UP].get());
        inputMgr.AddKeyboardCommand(SDLK_DOWN, m_Commands[MOVE_DOWN].get());
        inputMgr.AddKeyboardCommand(SDLK_LEFT, m_Commands[MOVE_LEFT].get());
        inputMgr.AddKeyboardCommand(SDLK_RIGHT, m_Commands[MOVE_RIGHT].get());
        inputMgr.AddKeyboardCommand(SDLK_p, m_Commands[ATTACK].get());
    }
    if (idx == 1)
    {
        inputMgr.AddKeyboardCommand(SDLK_w, m_Commands[MOVE_UP].get());
        inputMgr.AddKeyboardCommand(SDLK_s, m_Commands[MOVE_DOWN].get());
        inputMgr.AddKeyboardCommand(SDLK_a, m_Commands[MOVE_LEFT].get());
        inputMgr.AddKeyboardCommand(SDLK_d, m_Commands[MOVE_RIGHT].get());
        inputMgr.AddKeyboardCommand(SDLK_t, m_Commands[ATTACK].get());
    }
}
