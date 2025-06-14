#include "AIControllerComponent.h"
#include "PlayerComponent.h"
#include "IControllable.h"
#include "LevelGrid.h"

static constexpr auto GetLadderWorldPos =
    [](const LevelGrid& levelGrid, const LadderInfo& ladder, bool useTop)
{
    const auto& gridCell = useTop ? ladder.top : ladder.bottom;
    return levelGrid.GridToWorld(gridCell) +
           glm::vec2{ levelGrid.GRID_SIZE * 0.5f, levelGrid.GRID_SIZE };
};
static constexpr auto GetInputKeyFromVector =
    [](const glm::ivec2& uVector) -> InputKey
{
    if (uVector.x > 0)
        return InputKey::MoveRight;
    if (uVector.x < 0)
        return InputKey::MoveLeft;
    if (uVector.y > 0)
        return InputKey::MoveDown;
    if (uVector.y < 0)
        return InputKey::MoveUp;
    return InputKey::NONE;
};
using namespace AIControllerStates;

auto HorizontalTravelState::Update(AIControllerComponent& pController)
    -> AIState&
{
    auto& levelGrid = LevelGrid::GetInstance();
    glm::ivec2 aiGridPos = levelGrid.WorldToGrid(pController.GetPosition());
    glm::ivec2 playerGridPos =
        levelGrid.WorldToGrid(pController.GetPlayerPosition());

    // If on different floors, switch to vertical
    if (aiGridPos.y != playerGridPos.y)
    {
        return pController.GetVerticalTravelState();
    }

    // Simple horizontal chase
    glm::vec2 playerPos = pController.GetPlayerPosition();
    glm::vec2 aiPos = pController.GetPosition();

    if (abs(playerPos.x - aiPos.x) > 20.0f)
    {
        glm::vec2 targetPos = { playerPos.x, aiPos.y };
        pController.MoveTowardTarget(targetPos);
    }

    return *this;
}

auto VerticalTravelState::Update(AIControllerComponent& pController) -> AIState&
{
    auto& levelGrid = LevelGrid::GetInstance();
    glm::ivec2 aiGridPos = levelGrid.WorldToGrid(pController.GetPosition());
    glm::ivec2 playerGridPos =
        levelGrid.WorldToGrid(pController.GetPlayerPosition());

    // If on same floor, switch to horizontal
    if (aiGridPos.y == playerGridPos.y)
    {
        return pController.GetHorizontalTravelState();
    }

    // Simple vertical movement toward player
    glm::vec2 playerPos = pController.GetPlayerPosition();
    glm::vec2 aiPos = pController.GetPosition();

    if (abs(playerPos.y - aiPos.y) > 20.0f)
    {
        glm::vec2 targetPos = { aiPos.x, playerPos.y };
        pController.MoveTowardTarget(targetPos);
    }

    return *this;
}

AIControllerComponent::AIControllerComponent(dae::GameObject& owner,
                                             IControllable* pControlled,
                                             PlayerComponent* pPlayer) :
    BaseComponent(owner),
    m_pControlled(pControlled),
    m_pPlayer(pPlayer)
{}

void AIControllerComponent::Update()
{
    auto& newState = m_pCurrentState->Update(*this);
    if (&newState != m_pCurrentState)
        m_pCurrentState = &newState;
}

void AIControllerComponent::MoveTowardTarget(glm::vec2 target)
{
    auto& levelGrid = LevelGrid::GetInstance();
    glm::vec2 currentPos = GetPosition();
    glm::ivec2 gridPos = levelGrid.WorldToGrid(currentPos);
    glm::ivec2 targetGridPos = levelGrid.WorldToGrid(target);
    glm::vec2 direction = target - currentPos;

    if (glm::length(direction) < 0.01f)
        return;

    direction = glm::normalize(direction);

    if (abs(direction.x) > abs(direction.y))
    {
        if (GridMoveCommand::IsPositionedForWalking(
                currentPos, gridPos, targetGridPos))
        {
            m_pControlled->HandleInput(
                GetInputKeyFromVector({ direction.x > 0 ? 1 : -1, 0 }));
        }
    }
    else
    {
        if (GridMoveCommand::IsPositionedForClimbing(
                currentPos, gridPos, targetGridPos))
        {
            m_pControlled->HandleInput(
                GetInputKeyFromVector({ 0, direction.y > 0 ? 1 : -1 }));
        }
    }
}

auto AIControllerComponent::GetPosition() const -> glm::vec2
{
    const auto& levelGrid = LevelGrid::GetInstance();
    const glm::vec2& pos = GetOwner().GetWorldPosition();
    const glm::vec2 offset{ levelGrid.GRID_SIZE / 2.f,
                            2 * levelGrid.GRID_SIZE };
    return pos + offset;
}

auto AIControllerComponent::GetVerticalTravelState()
    -> AIControllerStates::VerticalTravelState&
{
    return m_VerticalTravelState;
}

auto AIControllerComponent::GetHorizontalTravelState()
    -> AIControllerStates::HorizontalTravelState&
{
    return m_HorizontalTravelState;
}

auto AIControllerComponent::GetCurrentState() const
    -> AIControllerStates::AIState*
{
    return m_pCurrentState;
}

auto AIControllerComponent::GetPlayerPosition() const -> glm::vec2
{
    const auto& levelGrid = LevelGrid::GetInstance();
    const glm::vec2& pos = m_pPlayer->GetPosition();
    const glm::vec2 offset{ levelGrid.GRID_SIZE / 2.f,
                            2 * levelGrid.GRID_SIZE };
    return pos + offset;
}

auto AIControllerComponent::GetDirectionToTarget(const glm::vec2& target,
                                                 const glm::vec2& position)
    -> Direction
{
    if (target[0] > position[0])
        return Direction::Right;
    if (target[0] < position[0])
        return Direction::Left;
    if (target[1] > position[1])
        return Direction::Down;
    return Direction::Up;
}
