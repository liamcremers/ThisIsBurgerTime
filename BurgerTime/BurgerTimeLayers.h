#pragma once
#include <cstdint>

enum class CollisionLayer
{
    Player = 0x0001,
    Food = 0x0002,
    Floor = 0x0004,
    BurgerPlate = 0x0008,
    Enemy = 0x0010,
    Ladder = 0x0020
};

constexpr uint16_t PLAYER_COLLISION_MASK =
    static_cast<uint16_t>(CollisionLayer::Food) |
    static_cast<uint16_t>(CollisionLayer::BurgerPlate);

constexpr uint16_t FLOOR_COLLISION_MASK =
    static_cast<uint16_t>(CollisionLayer::Player) |
    static_cast<uint16_t>(CollisionLayer::Enemy) |
    static_cast<uint16_t>(CollisionLayer::Food);

constexpr uint16_t FOOD_COLLISION_MASK =
    static_cast<uint16_t>(CollisionLayer::Player) |
    static_cast<uint16_t>(CollisionLayer::BurgerPlate);

constexpr uint16_t LADDER_COLLISION_MASK =
    static_cast<uint16_t>(CollisionLayer::Player) |
    static_cast<uint16_t>(CollisionLayer::Enemy) |
    static_cast<uint16_t>(CollisionLayer::Food);
