#pragma once
#include "IControllable.h"
#include "InputCommand.h"

#include <EngineInputComponent.h>
#include <Controller.h>
#include <BaseComponent.h>
#include <memory>
#include <unordered_map>

namespace dae
{
    class Controller;
    class GameObject;
}

class InputComponent : public dae::EngineInputComponent
{
    static constexpr unsigned int GAMEPAD_DPAD_U = 0x0001;
    static constexpr unsigned int GAMEPAD_DPAD_D = 0x0002;
    static constexpr unsigned int GAMEPAD_DPAD_L = 0x0004;
    static constexpr unsigned int GAMEPAD_DPAD_R = 0x0008;
    static constexpr unsigned int GAMEPAD_A = 0x1000;
    static constexpr auto PRESSED = dae::ButtonState::Pressed;
    static constexpr auto MOVE_UP = InputKey::MoveUp;
    static constexpr auto MOVE_DOWN = InputKey::MoveDown;
    static constexpr auto MOVE_LEFT = InputKey::MoveLeft;
    static constexpr auto MOVE_RIGHT = InputKey::MoveRight;
    static constexpr auto ATTACK = InputKey::Attack;

public:
    explicit InputComponent(dae::GameObject& parent,
                            unsigned long idx,
                            IControllable* controllable);
    ~InputComponent() override;
    const dae::Controller* GetController() const;

private:
    void SetUpKeyboardControls();
    void SetUpGamePadControls();
    void RemoveGamePadControls();

    unsigned long m_Index;
    std::unique_ptr<dae::Controller> m_pController;
    std::unordered_map<InputKey, std::unique_ptr<InputCommand>> m_Commands;
    IControllable* m_pControllable;

protected:
    void RegisterCommands() override;
    void UnregisterCommands() override;
};