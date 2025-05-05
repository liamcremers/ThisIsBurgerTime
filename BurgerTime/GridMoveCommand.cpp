#include "GridMoveCommand.h"
#include "LevelGrid.h"

#include <PhysicsComponent.h>
#include <ColliderComponent.h>

#ifdef DEBUG_RENDER
#include <DebugRenderer.h>
#endif // DEBUG_RENDER

GridMoveCommand::GridMoveCommand(dae::GameObject& pGameObject,
                                 glm::i8vec2 m_Direction,
                                 int speed) :
    GameObjectCommand{ &pGameObject },
    m_pPhysics{ GetGameObject()->GetComponent<dae::PhysicsComponent>() },
    m_pCollider{ GetGameObject()->GetComponent<dae::ColliderComponent>() },
    m_Speed{ speed },
    m_Direction{ m_Direction }
{
    assert(m_pPhysics and m_pCollider and
           "GridMoveCommand requires a ColliderComponent/PhysicsComponent "
           "to function");
}

void GridMoveCommand::Execute()
{
    static constexpr auto HALF_SIZE_FACTOR = 0.5f;

    auto& levelGrid = LevelGrid::GetInstance();
    const auto& playerPos = GetGameObject()->GetWorldPosition();
    auto playerSize = m_pCollider->GetSize();
    auto halfPlayerSize = playerSize * HALF_SIZE_FACTOR;

    glm::vec2 worldPos =
        playerPos + glm::vec2{ halfPlayerSize[0], playerSize[1] };
    glm::ivec2 gridPos = levelGrid.WorldToGrid(worldPos);
    glm::ivec2 targetGridPos =
        gridPos + glm::ivec2(m_Direction[0], m_Direction[1]);

    bool canMove{};

    if (IsPositionedForWalking(worldPos, gridPos, targetGridPos) or
        IsPositionedForClimbing(worldPos, gridPos, targetGridPos))
        canMove = true;

    if (canMove)
    {
        glm::vec2 currentPos = playerPos;

        if (m_Direction[0] != 0)
            currentPos[1] =
                levelGrid.GridToWorld(targetGridPos)[1] - playerSize[1];
        else if (m_Direction[1] != 0)
            currentPos[0] =
                levelGrid.GridToWorld(targetGridPos)[0] - (halfPlayerSize[0]);
        float deltaTime = dae::EngineTime::GetInstance().GetDeltaTime();
        glm::vec2 newPos =
            currentPos +
            glm::vec2(static_cast<float>(m_Direction[0] * m_Speed) * deltaTime,
                      static_cast<float>(m_Direction[1] * m_Speed) * deltaTime);
        m_pCollider->SetHasMoved(true);
        GetGameObject()->SetLocalPosition(newPos);
    }
}

void GridMoveCommand::Undo()
{
    if (auto* physics = GetGameObject()->GetComponent<dae::PhysicsComponent>())
    {
        auto velocity = physics->GetVelocity();
        if (m_Direction[0] != 0)
            velocity[0] = 0;
        if (m_Direction[1] != 0)
            velocity[1] = 0;
        physics->SetVelocity(velocity);
    }
}

void GridMoveCommand::SetSpeed(int speed) { m_Speed = speed; }

auto GridMoveCommand::IsPositionedForClimbing(const glm::vec2& playerPos,
                                              glm::ivec2& playerGridPos,
                                              glm::ivec2& targetGridPos) -> bool
{
    auto& levelGrid = LevelGrid::GetInstance();

    if (m_Direction[1] == 0)
        return false;

    // Adjust target grid position based on horizontal alignment
    glm::vec2 cellWorldPos = levelGrid.GridToWorld(playerGridPos);
    float xOffset = playerPos[0] - cellWorldPos[0];
    if (std::abs(xOffset - levelGrid.GRID_SIZE) < BUFFER)
    {
        playerGridPos[0] += 1;
        targetGridPos[0] += 1;
    }
    else if (std::abs(xOffset) > BUFFER)
        return false;

    auto leftCell =
        levelGrid.GetCellType(playerGridPos[0] - 1, playerGridPos[1]);
    auto rightCell = levelGrid.GetCellType(playerGridPos[0], playerGridPos[1]);
    auto leftTargetCell =
        levelGrid.GetCellType(targetGridPos[0] - 1, targetGridPos[1]);
    auto rightTargetCell =
        levelGrid.GetCellType(targetGridPos[0], targetGridPos[1]);

#ifdef DEBUG_RENDER
    dae::DebugRenderer::GetInstance().RenderRect(
        levelGrid.GridToWorld({ playerGridPos[0] - 1, playerGridPos[1] }),
        glm::vec2{ levelGrid.GRID_SIZE, levelGrid.GRID_SIZE });

    dae::DebugRenderer::GetInstance().RenderRect(
        levelGrid.GridToWorld({ playerGridPos[0], playerGridPos[1] }),
        glm::vec2{ levelGrid.GRID_SIZE, levelGrid.GRID_SIZE });

    dae::DebugRenderer::GetInstance().RenderRect(
        levelGrid.GridToWorld({ targetGridPos[0] - 1, targetGridPos[1] }),
        glm::vec2{ levelGrid.GRID_SIZE, levelGrid.GRID_SIZE },
        { 1, 0, 0, 1 });

    dae::DebugRenderer::GetInstance().RenderRect(
        levelGrid.GridToWorld({ targetGridPos[0], targetGridPos[1] }),
        glm::vec2{ levelGrid.GRID_SIZE, levelGrid.GRID_SIZE },
        { 1, 0, 0, 1 });
#endif // DEBUG_RENDER
    bool isCurrentStair = IsLadder({ leftCell, rightCell });
    bool isTargetStair = IsLadder({ leftTargetCell, rightTargetCell });

    return isCurrentStair or isTargetStair;
}

auto GridMoveCommand::IsPositionedForWalking(const glm::vec2& playerPos,
                                             glm::ivec2& playerGridPos,
                                             glm::ivec2& targetGridPos) -> bool
{
    auto& levelGrid = LevelGrid::GetInstance();

    if (m_Direction[0] == 0)
        return false;

    // Adjust target grid position on player position
    float yOffset = playerPos[1] - levelGrid.GridToWorld(playerGridPos)[1];
    if (std::abs(yOffset - levelGrid.GRID_SIZE) < BUFFER)
    {
        playerGridPos[1] += 1;
        targetGridPos[1] += 1;
    }
    else if (std::abs(yOffset) > BUFFER)
        return false;

    auto targetCell = levelGrid.GetCellType(targetGridPos[0], targetGridPos[1]);

#ifdef DEBUG_RENDER
    dae::DebugRenderer::GetInstance().RenderRect(
        levelGrid.GridToWorld({ playerGridPos[0], playerGridPos[1] }),
        glm::vec2{ levelGrid.GRID_SIZE, levelGrid.GRID_SIZE });
    dae::DebugRenderer::GetInstance().RenderRect(
        levelGrid.GridToWorld({ targetGridPos[0], targetGridPos[1] }),
        glm::vec2{ levelGrid.GRID_SIZE, levelGrid.GRID_SIZE },
        { 1, 0, 0, 1 });
#endif // DEBUG_RENDER

    return IsFloor({ targetCell });
}

auto GridMoveCommand::IsLadder(std::initializer_list<CellTypes> cellTypeList)
    -> bool
{
    return std::ranges::all_of(
        cellTypeList,
        [](const CellTypes& cellTypes)
        { return cellTypes.contains(ECellType::Ladder); });
}

auto GridMoveCommand::IsFloor(std::initializer_list<CellTypes> cellTypeList)
    -> bool
{
    return std::ranges::all_of(
        cellTypeList,
        [](const CellTypes& cellTypes)
        { return cellTypes.contains(ECellType::Floor); });
}