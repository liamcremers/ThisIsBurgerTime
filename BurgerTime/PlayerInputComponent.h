#pragma once
#include <BaseComponent.h>
#include <Command.h>
#include <vec2.hpp>

#include <memory>

namespace dae
{
    class LivesComponent;
    class ScoreComponent;
    class Controller;
}

class GridMoveCommand;

class PlayerInputComponent final : public dae::BaseComponent
{
#pragma region STATIC_CONSTANTS
    static constexpr glm::i8vec2 MoveUp{ 0, -1 };
    static constexpr glm::i8vec2 MoveDown{ 0, 1 };
    static constexpr glm::i8vec2 MoveLeft{ -1, 0 };
    static constexpr glm::i8vec2 MoveRight{ 1, 0 };

    static constexpr unsigned int XINPUT_GAMEPAD_DPAD_UP = 0x0001;
    static constexpr unsigned int XINPUT_GAMEPAD_DPAD_DOWN = 0x0002;
    static constexpr unsigned int XINPUT_GAMEPAD_DPAD_LEFT = 0x0004;
    static constexpr unsigned int XINPUT_GAMEPAD_DPAD_RIGHT = 0x0008;
    static constexpr unsigned int XINPUT_GAMEPAD_A = 0x1000;
    static constexpr unsigned int XINPUT_GAMEPAD_B = 0x2000;
    static constexpr unsigned int XINPUT_GAMEPAD_X = 0x4000;

    static constexpr int TEN_POINTS = 10;
    static constexpr int HUNDRED_POINTS = 100;
#pragma endregion
public:
    PlayerInputComponent(dae::GameObject& parent, unsigned long idx);
    virtual ~PlayerInputComponent();
    const dae::Controller* GetController() const;

    void SetCanMove(bool enableMovement);

    GridMoveCommand* GetMoveCommandUp() const;
    GridMoveCommand* GetMoveCommandDown() const;
    GridMoveCommand* GetMoveCommandLeft() const;
    GridMoveCommand* GetMoveCommandRight() const;

private:
    void SetUpKeyboardControls(unsigned long idx);

    std::unique_ptr<dae::Controller> m_pController{};
    std::unique_ptr<GridMoveCommand> m_pMoveCommandUp{};
    std::unique_ptr<GridMoveCommand> m_pMoveCommandDown{};
    std::unique_ptr<GridMoveCommand> m_pMoveCommandLeft{};
    std::unique_ptr<GridMoveCommand> m_pMoveCommandRight{};
};
