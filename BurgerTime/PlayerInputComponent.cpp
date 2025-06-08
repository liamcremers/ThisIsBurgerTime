#include "PlayerInputComponent.h"
#include "PlayerComponent.h"
#include "PlayerCommand.h"

#include <InputManager.h>
#include <Controller.h>
#include <GameObject.h>

#include <SDL.h>

using PlayerInputKeys::Attack;
#ifdef _DEBUG
using PlayerInputKeys::Die;
#endif // _DEBUG
using PlayerInputKeys::MoveDown;
using PlayerInputKeys::MoveLeft;
using PlayerInputKeys::MoveRight;
using PlayerInputKeys::MoveUp;

PlayerInputComponent::PlayerInputComponent(dae::GameObject& parent,
                                           unsigned long idx) :
    BaseComponent(parent),
    m_pController{ std::make_unique<dae::Controller>(idx) },
    m_pPlayerCommandUp{ std::make_unique<PlayerCommand>(parent, MoveUp) },
    m_pPlayerCommandDown{ std::make_unique<PlayerCommand>(parent, MoveDown) },
    m_pPlayerCommandLeft{ std::make_unique<PlayerCommand>(parent, MoveLeft) },
    m_pPlayerCommandRight{ std::make_unique<PlayerCommand>(parent, MoveRight) },
    m_pPlayerCommandAttack{ std::make_unique<PlayerCommand>(parent, Attack) }
#ifdef _DEBUG
    ,
    m_pPlayerCommandDie //
    { std::make_unique<PlayerCommand>(parent, PlayerInputKeys::Die) }
#endif // _DEBUG
{
    dae::InputManager::GetInstance().AddController(m_pController.get());
    m_pController->AddCommand(*m_pPlayerCommandUp, //
                              XINPUT_GAMEPAD_DPAD_UP,
                              dae::ButtonState::Pressed);
    m_pController->AddCommand(*m_pPlayerCommandDown,
                              XINPUT_GAMEPAD_DPAD_DOWN,
                              dae::ButtonState::Pressed);
    m_pController->AddCommand(*m_pPlayerCommandLeft,
                              XINPUT_GAMEPAD_DPAD_LEFT,
                              dae::ButtonState::Pressed);
    m_pController->AddCommand(*m_pPlayerCommandRight,
                              XINPUT_GAMEPAD_DPAD_RIGHT,
                              dae::ButtonState::Pressed);
    m_pController->AddCommand(
        *m_pPlayerCommandAttack, XINPUT_GAMEPAD_A, dae::ButtonState::Pressed);
#ifdef _DEBUG
    m_pController->AddCommand(
        *m_pPlayerCommandDie, XINPUT_GAMEPAD_B, dae::ButtonState::Pressed);
#endif // _DEBUG
    SetUpKeyboardControls(idx);
}

PlayerInputComponent::~PlayerInputComponent()
{
    m_pController->RemoveCommand(*m_pPlayerCommandUp, //
                                 XINPUT_GAMEPAD_DPAD_UP,
                                 dae::ButtonState::Pressed);
    m_pController->RemoveCommand(*m_pPlayerCommandDown,
                                 XINPUT_GAMEPAD_DPAD_DOWN,
                                 dae::ButtonState::Pressed);
    m_pController->RemoveCommand(*m_pPlayerCommandLeft,
                                 XINPUT_GAMEPAD_DPAD_LEFT,
                                 dae::ButtonState::Pressed);
    m_pController->RemoveCommand(*m_pPlayerCommandRight,
                                 XINPUT_GAMEPAD_DPAD_RIGHT,
                                 dae::ButtonState::Pressed);
    m_pController->RemoveCommand(
        *m_pPlayerCommandAttack, XINPUT_GAMEPAD_A, dae::ButtonState::Pressed);
#ifdef _DEBUG
    m_pController->RemoveCommand(
        *m_pPlayerCommandDie, XINPUT_GAMEPAD_B, dae::ButtonState::Pressed);
#endif // _DEBUG
}

auto PlayerInputComponent::GetController() const -> const dae::Controller*
{
    return m_pController.get();
}

void PlayerInputComponent::SetUpKeyboardControls(unsigned long idx)
{
    auto& inputMgr = dae::InputManager::GetInstance();
    if (idx == 0)
    {
        inputMgr.AddKeyboardCommand(SDLK_UP, m_pPlayerCommandUp.get());
        inputMgr.AddKeyboardCommand(SDLK_DOWN, m_pPlayerCommandDown.get());
        inputMgr.AddKeyboardCommand(SDLK_LEFT, m_pPlayerCommandLeft.get());
        inputMgr.AddKeyboardCommand(SDLK_RIGHT, m_pPlayerCommandRight.get());
        inputMgr.AddKeyboardCommand(SDLK_p, m_pPlayerCommandAttack.get());
#ifdef _DEBUG
        inputMgr.AddKeyboardCommand(SDLK_o, m_pPlayerCommandDie.get());
#endif // _DEBUG
    }
    if (idx == 1)
    {
        inputMgr.AddKeyboardCommand(SDLK_w, m_pPlayerCommandUp.get());
        inputMgr.AddKeyboardCommand(SDLK_s, m_pPlayerCommandDown.get());
        inputMgr.AddKeyboardCommand(SDLK_a, m_pPlayerCommandLeft.get());
        inputMgr.AddKeyboardCommand(SDLK_d, m_pPlayerCommandRight.get());
        inputMgr.AddKeyboardCommand(SDLK_t, m_pPlayerCommandAttack.get());
#ifdef _DEBUG
        inputMgr.AddKeyboardCommand(SDLK_r, m_pPlayerCommandDie.get());
#endif // _DEBUG
    }
}
