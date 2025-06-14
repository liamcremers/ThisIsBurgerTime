#pragma once
#include "Direction.h"
#include <glm.hpp>

enum class InputKey
{
    MoveLeft,
    MoveRight,
    MoveUp,
    MoveDown,
    Attack,
    NONE
};

static constexpr auto GetInputKeyFromDirection(Direction direction) -> InputKey
{
    switch (direction)
    {
    case Direction::Left:
        return InputKey::MoveLeft;
    case Direction::Right:
        return InputKey::MoveRight;
    case Direction::Up:
        return InputKey::MoveUp;
    case Direction::Down:
        return InputKey::MoveDown;
    default:
        return InputKey::NONE;
    }
};

class IControllable
{
public:
    virtual ~IControllable() = default;
    virtual void HandleInput(InputKey input) = 0;
    virtual bool Move(glm::vec2 direction) = 0;
};
