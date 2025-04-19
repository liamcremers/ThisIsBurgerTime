#pragma once
#include <ColliderComponent.h>
#include <GameObject.h>
#ifdef DEBUG_RENDER
#include <DebugRenderer.h>
#endif // DEBUG_RENDER

#include "Singleton.h"
#include <Scene.h>

enum class CellType
{
    Empty,
    Floor,
    Ladder,
    FloorAndLadder
};

class LevelGrid : public dae::Singleton<LevelGrid>
{
public:
    static constexpr int GRID_WIDTH = 30;
    static constexpr int GRID_HEIGHT = 29;
    static constexpr int GRID_SIZE = 16;

    CellType GetCellType(int gridX, int gridY) const
    {
        // Check bounds
        if (gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 &&
            gridY < GRID_HEIGHT)
            return m_LevelGrid[gridY][gridX];

        // Return Empty for out-of-bounds access
        return CellType::Empty;
    }

    void InitializeLevelGrid(const dae::Scene& scene, glm::ivec2 startPos)
    {
        m_StartOffset = startPos;

        // Clear all cells
        for (int y = 0; y < GRID_HEIGHT; ++y)
            for (int x = 0; x < GRID_WIDTH; ++x)
                m_LevelGrid[y][x] = CellType::Empty;

        // Process objects from the scene
        for (auto obj : scene.GetGameObjects())
        {
            auto* collider = obj->GetComponent<dae::ColliderComponent>();
            if (!collider)
                continue;

            // Convert world position to grid position (accounting for offset)
            glm::vec2 pos = obj->GetWorldPosition();

            auto gridPos = WorldToGrid(pos);

            // Check if position is in bounds
            if (gridPos.x >= 0 && gridPos.x < GRID_WIDTH && gridPos.y >= 0 &&
                gridPos.y < GRID_HEIGHT)
            {
                // Set correct cell type
                if (collider->GetLayer() &
                    static_cast<uint16_t>(BurgerTime::CollisionLayer::Floor))
                {
                    if (m_LevelGrid[gridPos.y][gridPos.x] == CellType::Ladder)
                        m_LevelGrid[gridPos.y][gridPos.x] =
                            CellType::FloorAndLadder;
                    else
                        m_LevelGrid[gridPos.y][gridPos.x] = CellType::Floor;
                }
                if (collider->GetLayer() &
                    static_cast<uint16_t>(BurgerTime::CollisionLayer::Ladder))
                {
                    if (m_LevelGrid[gridPos.y][gridPos.x] == CellType::Floor)
                        m_LevelGrid[gridPos.y][gridPos.x] =
                            CellType::FloorAndLadder;
                    else
                        m_LevelGrid[gridPos.y][gridPos.x] = CellType::Ladder;
                }
            }
        }
    }

    [[nodiscard]] glm::ivec2 WorldToGrid(const glm::vec2& worldPos) const
    {
        auto pos = worldPos - glm::vec2{ m_StartOffset };
        return glm::ivec2(static_cast<int>(pos.x / GRID_SIZE),
                          static_cast<int>(pos.y / GRID_SIZE));
    }

    [[nodiscard]] glm::vec2 GridToWorld(const glm::ivec2& gridPos) const
    {
        return (glm::vec2{ gridPos.x * GRID_SIZE, gridPos.y * GRID_SIZE } +
                glm::vec2{ m_StartOffset });
    }
#ifdef DEBUG_RENDER
    void DrawCurrentCell(int gridX, int gridY) const
    {
        glm::vec2 worldPos = GridToWorld({ gridX, gridY });
        dae::DebugRenderer::GetInstance().RenderRect(
            worldPos, { GRID_SIZE, GRID_SIZE }, { 1.f, 0.f, 0.f, 1.f });
    }

    void DrawGrid() const
    {
        for (int y = 0; y < GRID_HEIGHT; ++y)
            for (int x = 0; x < GRID_WIDTH; ++x)
            {
                CellType cellType = m_LevelGrid[y][x];
                glm::vec2 worldPos = GridToWorld({ x, y });
                auto color = glm::vec4{ 1.f, 1.f, 1.f, 1.f };
                switch (cellType)
                {
                case CellType::Empty:
                    color = glm::vec4{ 1.f, 1.f, 1.f, 1.f };
                    break;
                case CellType::Floor:
                    color = glm::vec4{ 0.f, 1.f, 0.f, 1.f };
                    break;
                case CellType::Ladder:
                    color = glm::vec4{ 0.f, 0.f, 1.f, 1.f };
                    break;
                case CellType::FloorAndLadder:
                    color = glm::vec4{ 1.f, 1.f, 0.f, 1.f };
                    break;
                default:
                    color = glm::vec4{ 1.f, 1.f, 1.f, 1.f };
                    break;
                }
                dae::DebugRenderer::GetInstance().RenderRect(
                    worldPos, { GRID_SIZE, GRID_SIZE }, color);
            }
    }
#endif // DEBUG_RENDER

private:
    friend class Singleton<LevelGrid>;
    LevelGrid() = default;

    CellType m_LevelGrid[GRID_HEIGHT][GRID_WIDTH]{};
    glm::ivec2 m_StartOffset{};
};
