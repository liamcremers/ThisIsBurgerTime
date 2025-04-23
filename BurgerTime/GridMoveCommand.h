#pragma once
#include <Command.h>
#include <glm.hpp>
#include <set>

namespace dae
{
    class GameObject;
    class PhysicsComponent;
    class ColliderComponent;
}
enum class ECellType;
using CellTypes = std::set<ECellType>;

class GridMoveCommand : public dae::GameObjectCommand
{
    static constexpr int BUFFER{ 6 };

public:
    GridMoveCommand(dae::GameObject& pGameObject,
                    glm::i8vec2 direction,
                    int speed = 100);
    ~GridMoveCommand() = default;
    void Execute() override;
    void Undo() override;

    void SetSpeed(int speed) { m_Speed = speed; };

private:
    bool IsPositionedForClimbing(const glm::vec2& playerPos,
                                 glm::ivec2& playerGridPos,
                                 glm::ivec2& targetGridPos);
    bool IsPositionedForWalking(const glm::vec2& playerPos,
                                glm::ivec2& playerGridPos,
                                glm::ivec2& targetGridPos);
    bool IsLadder(std::initializer_list<CellTypes> cellTypeList);
    bool IsFloor(std::initializer_list<CellTypes> cellTypeList);

    dae::PhysicsComponent* m_pPhysics{ nullptr };
    dae::ColliderComponent* m_pCollider{ nullptr };
    int m_Speed{ 100 };
    glm::i8vec2 m_Direction{};
};
