#pragma once
#include <glm.hpp>

enum class Direction
{
    Left,
    Right,
    Up,
    Down,
    None
};

const struct DirectionVec
{
    static constexpr glm::vec2 Left{ -1, 0 };
    static constexpr glm::vec2 Right{ 1, 0 };
    static constexpr glm::vec2 Up{ 0, -1 };
    static constexpr glm::vec2 Down{ 0, 1 };
    static constexpr glm::vec2 None{ 0, 0 };
};

static constexpr glm::vec2 FromEnum(Direction dir)
{
    switch (dir)
    {
    case Direction::Left:
        return DirectionVec::Left;
    case Direction::Right:
        return DirectionVec::Right;
    case Direction::Up:
        return DirectionVec::Up;
    case Direction::Down:
        return DirectionVec::Down;
    default:
        return DirectionVec::None;
    }
}

static constexpr Direction FromVec(const glm::vec2& vec)
{
    if (vec == DirectionVec::Left)
        return Direction::Left;
    if (vec == DirectionVec::Right)
        return Direction::Right;
    if (vec == DirectionVec::Up)
        return Direction::Up;
    if (vec == DirectionVec::Down)
        return Direction::Down;
    return Direction::None;
};
