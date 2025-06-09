#pragma once
#include <ColliderComponent.h>
#include <GameObject.h>
#ifdef DEBUG_RENDER
#include <DebugRenderer.h>
#endif // DEBUG_RENDER

#include "BurgerTimeLayers.h"
#include "Singleton.h"
#include <Scene.h>
#include <unordered_set>
#include <array>

enum class ECellType
{
    Empty,
    Floor,
    Ladder,
    Food,
    BurgerPlate
};
using CellTypes = std::unordered_set<ECellType>;

class LevelGrid : public dae::Singleton<LevelGrid>
{
public:
    static constexpr int GRID_WIDTH = 30;
    static constexpr int GRID_HEIGHT = 29;
    static constexpr int GRID_SIZE = 16;
    using GridRow = std::array<CellTypes, GRID_WIDTH>;
    using Grid = std::array<GridRow, GRID_HEIGHT>;

    void InitializeLevelGrid(const dae::Scene& scene, glm::ivec2 startPos);

    [[nodiscard]]
    auto GetCellType(const int gridX, const int gridY) const -> CellTypes;
    [[nodiscard]]
    bool IsInBounds(const int gridX, const int gridY) const;
    [[nodiscard]]
    auto WorldToGrid(const glm::vec2& worldPos) const -> glm::ivec2;
    [[nodiscard]]
    auto GridToWorld(const glm::ivec2& gridPos) const -> glm::vec2;

#ifdef DEBUG_RENDER
    void DrawCurrentCell(int gridX, int gridY) const;
    void DrawGrid() const;
#endif // DEBUG_RENDER

private:
    friend class Singleton<LevelGrid>;
    LevelGrid() = default;

    Grid m_LevelGrid{};
    glm::ivec2 m_StartOffset{};
};
