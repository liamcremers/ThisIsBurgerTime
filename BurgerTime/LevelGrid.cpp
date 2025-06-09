#include "LevelGrid.h"
#include <ranges>
#include <algorithm>

void LevelGrid::InitializeLevelGrid(const dae::Scene& scene,
                                    glm::ivec2 startPos)
{
    m_StartOffset = startPos;

    // Clear all cells
    for (auto& y : m_LevelGrid)
        for (auto& x : y)
            x = { ECellType::Empty };

    for (auto* obj : scene.GetGameObjects())
    {
        auto* collider = obj->GetComponent<dae::ColliderComponent>();
        if (!collider)
            continue;

        glm::vec2 pos = obj->GetWorldPosition();
        const auto gridPos = WorldToGrid(pos);

        if (IsInBounds(gridPos[0], gridPos[1]))
        {
            auto& cell = m_LevelGrid.at(static_cast<size_t>(gridPos[1]))
                             .at(static_cast<size_t>(gridPos[0]));

            uint16_t layer = collider->GetLayer();

            switch (static_cast<CollisionLayer>(layer))
            {
            case CollisionLayer::Floor:
                cell.insert(ECellType::Floor);
                break;
            case CollisionLayer::Ladder:
                cell.insert(ECellType::Ladder);
                break;
            case CollisionLayer::BurgerPart:
                cell.insert(ECellType::Food);
                break;
            case CollisionLayer::BurgerPlate:
                cell.insert(ECellType::BurgerPlate);
                break;
            default:
                break;
            }
        }
    }
}

[[nodiscard]]
auto LevelGrid::GetCellType(const int gridX, const int gridY) const -> CellTypes
{
    if (IsInBounds(gridX, gridY))
        return m_LevelGrid.at(static_cast<size_t>(gridY))
            .at(static_cast<size_t>(gridX));

    // Return Empty for out-of-bounds access
    return CellTypes{ ECellType::Empty };
}

[[nodiscard]] auto LevelGrid::IsInBounds(const int gridX, const int gridY) const
    -> bool
{
    return gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 &&
           gridY < GRID_HEIGHT;
}

[[nodiscard]] auto LevelGrid::WorldToGrid(const glm::vec2& worldPos) const
    -> glm::ivec2

{
    auto pos = worldPos - glm::vec2{ m_StartOffset };
    return { static_cast<int>(pos[0] / GRID_SIZE),
             static_cast<int>(pos[1] / GRID_SIZE) };
}

[[nodiscard]] auto LevelGrid::GridToWorld(const glm::ivec2& gridPos) const
    -> glm::vec2
{
    return (glm::vec2{ gridPos[0] * GRID_SIZE, gridPos[1] * GRID_SIZE } +
            glm::vec2{ m_StartOffset });
}

#ifdef DEBUG_RENDER
void LevelGrid::DrawCurrentCell(int gridX, int gridY) const
{
    glm::vec2 worldPos = GridToWorld({ gridX, gridY });
    dae::DebugRenderer::GetInstance().RenderRect(
        worldPos, { GRID_SIZE, GRID_SIZE }, { 1.f, 0.f, 0.f, 1.f });
}

void LevelGrid::DrawGrid() const
{
    for (int y = 0; y < GRID_HEIGHT; ++y)
        for (int x = 0; x < GRID_WIDTH; ++x)
        {
            const auto& cellTypes = m_LevelGrid[y][x];
            glm::vec2 worldPos = GridToWorld({ x, y });
            auto color = glm::vec4{ 1.f, 1.f, 1.f, 1.f };
            if (cellTypes.contains(ECellType::BurgerPlate))
            {
                color = { 1.f, 1.f, 1.f, 1.f };
                dae::DebugRenderer::GetInstance().RenderRect(
                    worldPos, { GRID_SIZE, GRID_SIZE }, color);
            }
        }
}
#endif // DEBUG_RENDER
