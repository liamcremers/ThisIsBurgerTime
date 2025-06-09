#include "EnemyInputComponent.h"
#include "EnemyComponent.h"
#include "EnemyCommand.h"

#include <InputManager.h>
#include <Controller.h>
#include <GameObject.h>

#include <SDL.h>

#ifdef _DEBUG
using EnemyInputKeys::Attack;
using EnemyInputKeys::Die;
#endif // _DEBUG
using EnemyInputKeys::MoveDown;
using EnemyInputKeys::MoveLeft;
using EnemyInputKeys::MoveRight;
using EnemyInputKeys::MoveUp;

EnemyInputComponent::EnemyInputComponent(dae::GameObject& parent,
                                         unsigned long idx) :
    BaseComponent(parent),
    m_pController{ std::make_unique<dae::Controller>(idx) },
    m_pEnemyCommandUp{ std::make_unique<EnemyCommand>(parent, MoveUp) },
    m_pEnemyCommandDown{ std::make_unique<EnemyCommand>(parent, MoveDown) },
    m_pEnemyCommandLeft{ std::make_unique<EnemyCommand>(parent, MoveLeft) },
    m_pEnemyCommandRight{ std::make_unique<EnemyCommand>(parent, MoveRight) }
#ifdef _DEBUG
    ,
    m_pEnemyCommandAttack{ std::make_unique<EnemyCommand>(parent, Attack) },
    m_pEnemyCommandDie //
    { std::make_unique<EnemyCommand>(parent, EnemyInputKeys::Die) }
#endif // _DEBUG
{
    dae::InputManager::GetInstance().AddController(m_pController.get());
    m_pController->AddCommand(*m_pEnemyCommandUp, //
                              XINPUT_GAMEPAD_DPAD_UP,
                              dae::ButtonState::Pressed);
    m_pController->AddCommand(*m_pEnemyCommandDown,
                              XINPUT_GAMEPAD_DPAD_DOWN,
                              dae::ButtonState::Pressed);
    m_pController->AddCommand(*m_pEnemyCommandLeft,
                              XINPUT_GAMEPAD_DPAD_LEFT,
                              dae::ButtonState::Pressed);
    m_pController->AddCommand(*m_pEnemyCommandRight,
                              XINPUT_GAMEPAD_DPAD_RIGHT,
                              dae::ButtonState::Pressed);
#ifdef _DEBUG
    m_pController->AddCommand(
        *m_pEnemyCommandAttack, XINPUT_GAMEPAD_A, dae::ButtonState::Pressed);
    m_pController->AddCommand(
        *m_pEnemyCommandDie, XINPUT_GAMEPAD_B, dae::ButtonState::Pressed);
#endif // _DEBUG
    SetUpKeyboardControls(idx);
}

EnemyInputComponent::~EnemyInputComponent()
{
    m_pController->RemoveCommand(*m_pEnemyCommandUp, //
                                 XINPUT_GAMEPAD_DPAD_UP,
                                 dae::ButtonState::Pressed);
    m_pController->RemoveCommand(*m_pEnemyCommandDown,
                                 XINPUT_GAMEPAD_DPAD_DOWN,
                                 dae::ButtonState::Pressed);
    m_pController->RemoveCommand(*m_pEnemyCommandLeft,
                                 XINPUT_GAMEPAD_DPAD_LEFT,
                                 dae::ButtonState::Pressed);
    m_pController->RemoveCommand(*m_pEnemyCommandRight,
                                 XINPUT_GAMEPAD_DPAD_RIGHT,
                                 dae::ButtonState::Pressed);
#ifdef _DEBUG
    m_pController->RemoveCommand(
        *m_pEnemyCommandAttack, XINPUT_GAMEPAD_A, dae::ButtonState::Pressed);
    m_pController->RemoveCommand(
        *m_pEnemyCommandDie, XINPUT_GAMEPAD_B, dae::ButtonState::Pressed);
#endif // _DEBUG
}

auto EnemyInputComponent::GetController() const -> const dae::Controller*
{
    return m_pController.get();
}

void EnemyInputComponent::SetUpKeyboardControls(unsigned long idx)
{
    auto& inputMgr = dae::InputManager::GetInstance();
    if (idx == 0)
    {
        inputMgr.AddKeyboardCommand(SDLK_UP, m_pEnemyCommandUp.get());
        inputMgr.AddKeyboardCommand(SDLK_DOWN, m_pEnemyCommandDown.get());
        inputMgr.AddKeyboardCommand(SDLK_LEFT, m_pEnemyCommandLeft.get());
        inputMgr.AddKeyboardCommand(SDLK_RIGHT, m_pEnemyCommandRight.get());
#ifdef _DEBUG
        inputMgr.AddKeyboardCommand(SDLK_p, m_pEnemyCommandAttack.get());
        inputMgr.AddKeyboardCommand(SDLK_o, m_pEnemyCommandDie.get());
#endif // _DEBUG
    }
    if (idx == 1)
    {
        inputMgr.AddKeyboardCommand(SDLK_w, m_pEnemyCommandUp.get());
        inputMgr.AddKeyboardCommand(SDLK_s, m_pEnemyCommandDown.get());
        inputMgr.AddKeyboardCommand(SDLK_a, m_pEnemyCommandLeft.get());
        inputMgr.AddKeyboardCommand(SDLK_d, m_pEnemyCommandRight.get());
#ifdef _DEBUG
        inputMgr.AddKeyboardCommand(SDLK_t, m_pEnemyCommandAttack.get());
        inputMgr.AddKeyboardCommand(SDLK_r, m_pEnemyCommandDie.get());
#endif // _DEBUG
    }
}
