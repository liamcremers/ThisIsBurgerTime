#pragma once
#include <ColliderComponent.h>
#include <GameObject.h>
#ifdef DEBUG_RENDER
#include <DebugRenderer.h>
#endif // DEBUG_RENDER

#include "BurgerTimeLayers.h"
#include "Singleton.h"
#include <Scene.h>
#include <set>

enum class ECellType
{
    Empty,
    Floor,
    Ladder,
    Food
};
using CellTypes = std::set<ECellType>;

class LevelGrid : public dae::Singleton<LevelGrid>
{
public:
    static constexpr int GRID_WIDTH = 30;
    static constexpr int GRID_HEIGHT = 29;
    static constexpr int GRID_SIZE = 16;

    CellTypes GetCellType(int gridX, int gridY) const
    {
        // Check bounds
        if (gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 &&
            gridY < GRID_HEIGHT)
            return m_LevelGrid[gridY][gridX];

        // Return Empty for out-of-bounds access
        return CellTypes{ ECellType::Empty };
    }

    void InitializeLevelGrid(const dae::Scene& scene, glm::ivec2 startPos)
    {
        m_StartOffset = startPos;

        // Clear all cells
        for (int y = 0; y < GRID_HEIGHT; ++y)
            for (int x = 0; x < GRID_WIDTH; ++x)
                m_LevelGrid[y][x] = { ECellType::Empty };

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
                auto& cell = m_LevelGrid[gridPos.y][gridPos.x];
                uint16_t layer = collider->GetLayer();

                // For each possible layer, check and insert the corresponding cell type
                switch (static_cast<BurgerTime::CollisionLayer>(layer))
                {
                case BurgerTime::CollisionLayer::Floor:
                    cell.insert(ECellType::Floor);
                    break;
                case BurgerTime::CollisionLayer::Ladder:
                    cell.insert(ECellType::Ladder);
                    break;
                case BurgerTime::CollisionLayer::Food:
                    cell.insert(ECellType::Food);
                    break;
                // Add more cases as needed
                default:
                    break;
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
                //auto cellType = m_LevelGrid[y][x];
                //glm::vec2 worldPos = GridToWorld({ x, y });
                //auto color = glm::vec4{ 1.f, 1.f, 1.f, 1.f };
                //switch (cellType)
                //{
                ////case CellType::Empty:
                ////    color = glm::vec4{ 1.f, 1.f, 1.f, 1.f };
                ////    break;
                ////case CellType::Floor:
                ////    color = glm::vec4{ 0.f, 1.f, 0.f, 1.f };
                ////    break;
                ////case CellType::Ladder:
                ////    color = glm::vec4{ 0.f, 0.f, 1.f, 1.f };
                ////    break;
                ////case CellType::FloorAndLadder:
                ////    color = glm::vec4{ 1.f, 1.f, 0.f, 1.f };
                ////    break;
                //case CellType::Food:
                //    color = glm::vec4{ 0.f, 0.f, 1.f, 1.f };
                //    break;
                //default:
                //    continue;
                //    break;
                //}
                //dae::DebugRenderer::GetInstance().RenderRect(
                //    worldPos, { GRID_SIZE, GRID_SIZE }, color);
            }
    }
#endif // DEBUG_RENDER

private:
    friend class Singleton<LevelGrid>;
    LevelGrid() = default;

    CellTypes m_LevelGrid[GRID_HEIGHT][GRID_WIDTH]{};
    glm::ivec2 m_StartOffset{};
};
