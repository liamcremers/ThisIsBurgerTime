#pragma once
enum class InputKey
{
    MoveLeft,
    MoveRight,
    MoveUp,
    MoveDown,
    Attack
};

class IControllable
{
public:
    virtual ~IControllable() = default;
    virtual void HandleInput(InputKey input) = 0;
};
