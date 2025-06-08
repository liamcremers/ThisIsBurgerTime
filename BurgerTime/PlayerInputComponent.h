#pragma once
#include <BaseComponent.h>
#include <Command.h>
#include <glm.hpp>
#include <memory>

namespace dae
{
    class Controller;
}
class PlayerCommand;

class PlayerInputComponent final : public dae::BaseComponent
{
#pragma region STATIC_CONSTANTS_XINPUT
    static constexpr unsigned int XINPUT_GAMEPAD_DPAD_UP = 0x0001;
    static constexpr unsigned int XINPUT_GAMEPAD_DPAD_DOWN = 0x0002;
    static constexpr unsigned int XINPUT_GAMEPAD_DPAD_LEFT = 0x0004;
    static constexpr unsigned int XINPUT_GAMEPAD_DPAD_RIGHT = 0x0008;
    static constexpr unsigned int XINPUT_GAMEPAD_A = 0x1000;
    static constexpr unsigned int XINPUT_GAMEPAD_B = 0x2000;
    static constexpr unsigned int XINPUT_GAMEPAD_X = 0x4000;
#pragma endregion
public:
    PlayerInputComponent(dae::GameObject& parent, unsigned long idx);
    virtual ~PlayerInputComponent();
    const dae::Controller* GetController() const;

private:
    void SetUpKeyboardControls(unsigned long idx);

    std::unique_ptr<dae::Controller> m_pController{};
    std::unique_ptr<PlayerCommand> m_pPlayerCommandUp{};
    std::unique_ptr<PlayerCommand> m_pPlayerCommandDown{};
    std::unique_ptr<PlayerCommand> m_pPlayerCommandLeft{};
    std::unique_ptr<PlayerCommand> m_pPlayerCommandRight{};
    std::unique_ptr<PlayerCommand> m_pPlayerCommandAttack{};
#ifdef _DEBUG
    std::unique_ptr<PlayerCommand>
        m_pPlayerCommandDie{}; // todo: remove this is for testing purposes only
#endif // DEBUG
};
