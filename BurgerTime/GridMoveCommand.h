#pragma once
#include <Command.h>
#include <glm.hpp>
#include <unordered_set>
#include <Observer.h>

namespace dae
{
    class GameObject;
    class PhysicsComponent;
    class ColliderComponent;
}
enum class ECellType;
using CellTypes = std::unordered_set<ECellType>;

class GridMoveCommand : public dae::GameObjectCommand
{
    static constexpr int BUFFER{ 6 };

public:
    GridMoveCommand(dae::GameObject& pGameObject,
                    glm::i8vec2 direction,
                    int speed = 100);
    bool ExecuteWithCheck();
    void Execute() override;
    void Undo() override;
    void SetSpeed(int speed);
    void SetCanMove(bool isMoving);

    dae::Subject& GetMoveCommandSubject() { return m_GridMoveSubject; }

private:
    dae::Subject m_GridMoveSubject{};
    [[nodiscard]]
    auto IsPositionedForClimbing(const glm::vec2& playerPos,
                                 glm::ivec2& playerGridPos,
                                 glm::ivec2& targetGridPos) -> bool;
    [[nodiscard]]
    auto IsPositionedForWalking(const glm::vec2& playerPos,
                                glm::ivec2& playerGridPos,
                                glm::ivec2& targetGridPos) -> bool;
    [[nodiscard]]
    auto IsLadder(std::initializer_list<CellTypes> cellTypeList) -> bool;
    [[nodiscard]]
    auto IsFloor(std::initializer_list<CellTypes> cellTypeList) -> bool;

    dae::PhysicsComponent* m_pPhysics{ nullptr };
    dae::ColliderComponent* m_pCollider{ nullptr };
    int m_Speed{ 100 };
    glm::i8vec2 m_Direction{};
    bool m_CanMove{ true };
};
