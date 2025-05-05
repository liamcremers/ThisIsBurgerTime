#pragma once
#include <BaseComponent.h>
#include <Command.h>

#include <glm.hpp>
//#include <glm.hpp>

#include <memory>
#include <vector>

namespace dae
{
    class LivesComponent;
    class ScoreComponent;
    class Controller;
}

class GridMoveCommand;

class SelfDamageCommand : public dae::GameObjectCommand
{
public:
    SelfDamageCommand(dae::GameObject& pGameObject);
    virtual ~SelfDamageCommand() = default;
    void Execute() override;
    void Undo() override {};

private:
    dae::LivesComponent* m_pLivesComp;
};

class AddPointsCommand : public dae::GameObjectCommand
{
public:
    AddPointsCommand(dae::GameObject& pGameObject, int pointsAdded);
    virtual ~AddPointsCommand() = default;
    void Execute() override;
    void Undo() override {};

private:
    dae::ScoreComponent* m_pScoreComp;
    int m_PointsAdded{};
};

class PlayerInputComponent final : public dae::BaseComponent
{
public:
    PlayerInputComponent(dae::GameObject& parent, unsigned long idx);
    virtual ~PlayerInputComponent();
    const dae::Controller* GetController() const;

    void SetSpeed(int speed);
#ifdef DEBUG_RENDER
    void Render() const override;
#endif // DEBUG_RENDER
private:
    void SetUpKeyboardControls(unsigned long idx);

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

    std::unique_ptr<dae::Controller> m_pController{};
    std::unique_ptr<GridMoveCommand> m_pMoveCommandUp{};
    std::unique_ptr<GridMoveCommand> m_pMoveCommandDown{};
    std::unique_ptr<GridMoveCommand> m_pMoveCommandLeft{};
    std::unique_ptr<GridMoveCommand> m_pMoveCommandRight{};
    std::unique_ptr<SelfDamageCommand> m_pSelfDamageCommand;
    std::unique_ptr<AddPointsCommand> m_pAdd100PointsCommand;
    std::unique_ptr<AddPointsCommand> m_pAdd10PointsCommand;
};
