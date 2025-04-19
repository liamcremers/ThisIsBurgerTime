#include "PlayerComponent.h"
#include "InputManager.h"
#include "Controller.h"

#include "EngineTime.h"
#include "GameObject.h"
#include "PhysicsComponent.h"
#include "ColliderComponent.h"
#include "CollisionSystem.h"

#ifdef DEBUG_RENDER
#include "DebugRenderer.h"
#endif // DEBUG_RENDER

#include "BurgerTimeLayers.h"
#include "LevelGrid.h"

#include <SDL.h>

PlayerInputComponent::PlayerInputComponent(dae::GameObject& parent,
                                           unsigned long idx) :
    BaseComponent(parent),
    m_pController{ std::make_unique<dae::Controller>(idx) },
    m_pMoveCommandUp{ std::make_unique<GridMoveCommand>(parent, MoveUp) },
    m_pMoveCommandDown{ std::make_unique<GridMoveCommand>(parent, MoveDown) },
    m_pMoveCommandLeft{ std::make_unique<GridMoveCommand>(parent, MoveLeft) },
    m_pMoveCommandRight{ std::make_unique<GridMoveCommand>(parent, MoveRight) },
    m_pSelfDamageCommand{ std::make_unique<SelfDamageCommand>(parent) },
    m_pAdd100PointsCommand{
        std::make_unique<AddPointsCommand>(parent, HUNDRED_POINTS)
    },
    m_pAdd10PointsCommand{ std::make_unique<AddPointsCommand>(parent,
                                                              TEN_POINTS) }

{
    dae::InputManager::GetInstance().AddController(m_pController.get());
    m_pController->AddCommand(
        *m_pMoveCommandUp, XINPUT_GAMEPAD_DPAD_UP, dae::ButtonState::Pressed);
    m_pController->AddCommand(*m_pMoveCommandDown,
                              XINPUT_GAMEPAD_DPAD_DOWN,
                              dae::ButtonState::Pressed);
    m_pController->AddCommand(*m_pMoveCommandLeft,
                              XINPUT_GAMEPAD_DPAD_LEFT,
                              dae::ButtonState::Pressed);
    m_pController->AddCommand(*m_pMoveCommandRight,
                              XINPUT_GAMEPAD_DPAD_RIGHT,
                              dae::ButtonState::Pressed);
    m_pController->AddCommand(*m_pSelfDamageCommand,
                              XINPUT_GAMEPAD_X,
                              dae::ButtonState::DownThisFrame);
    m_pController->AddCommand(*m_pAdd100PointsCommand,
                              XINPUT_GAMEPAD_A,
                              dae::ButtonState::DownThisFrame);
    m_pController->AddCommand(*m_pAdd10PointsCommand,
                              XINPUT_GAMEPAD_B,
                              dae::ButtonState::DownThisFrame);

    SetUpKeyboardControls(idx);

    //GetOwner().GetComponent<ColliderComponent>()->SubscribeToBeginOverlap(
    //    [this](const ColliderComponent& other)
    //    {
    //        if ((other.GetLayer() &
    //             static_cast<uint16_t>(
    //                 BurgerTime::CollisionLayer::Floor)) != 0)
    //        {
    //            m_pMoveCommandLeft->SetReady(true);
    //            m_pMoveCommandRight->SetReady(true);
    //            GetOwner()
    //                .GetComponent<dae::PhysicsComponent>()
    //                ->SetUseGravity(false);
    //        }
    //    });
    //GetOwner().GetComponent<ColliderComponent>()->SubscribeToEndOverlap(
    //    [this](const ColliderComponent& other)
    //    {
    //        if ((other.GetLayer() &
    //             static_cast<uint16_t>(
    //                 BurgerTime::CollisionLayer::Floor)) != 0)
    //        {
    //            //m_pMoveCommandLeft->SetReady(false);
    //            //m_pMoveCommandRight->SetReady(false);
    //        }
    //    });
}

PlayerInputComponent::~PlayerInputComponent()
{
    m_pController->RemoveCommand(
        *m_pMoveCommandUp, XINPUT_GAMEPAD_DPAD_UP, dae::ButtonState::Pressed);
    m_pController->RemoveCommand(*m_pMoveCommandDown,
                                 XINPUT_GAMEPAD_DPAD_DOWN,
                                 dae::ButtonState::Pressed);
    m_pController->RemoveCommand(*m_pMoveCommandLeft,
                                 XINPUT_GAMEPAD_DPAD_LEFT,
                                 dae::ButtonState::Pressed);
    m_pController->RemoveCommand(*m_pMoveCommandRight,
                                 XINPUT_GAMEPAD_DPAD_RIGHT,
                                 dae::ButtonState::Pressed);
    m_pController->RemoveCommand(*m_pSelfDamageCommand,
                                 XINPUT_GAMEPAD_X,
                                 dae::ButtonState::DownThisFrame);
    m_pController->RemoveCommand(*m_pAdd100PointsCommand,
                                 XINPUT_GAMEPAD_A,
                                 dae::ButtonState::DownThisFrame);
    m_pController->RemoveCommand(*m_pAdd10PointsCommand,
                                 XINPUT_GAMEPAD_B,
                                 dae::ButtonState::DownThisFrame);
}

auto PlayerInputComponent::GetController() const -> const dae::Controller*
{
    return m_pController.get();
}

void PlayerInputComponent::SetSpeed(int speed)
{
    m_pMoveCommandUp->SetSpeed(speed);
    m_pMoveCommandDown->SetSpeed(speed);
    m_pMoveCommandLeft->SetSpeed(speed);
    m_pMoveCommandRight->SetSpeed(speed);
}

#ifdef DEBUG_RENDER
void PlayerInputComponent::Render() const
{
    constexpr glm::vec2 DEBUG_SIZE{ 6, 6 };
    auto pos = GetOwner().GetWorldPosition();
    dae::DebugRenderer::GetInstance().RenderRect(
        pos + (-DEBUG_SIZE / 2.f), DEBUG_SIZE, { 1, 0, 0, 1 });

    //auto& levelGrid = LevelGrid::GetInstance();
    //levelGrid.DrawGrid();

    //glm::ivec2 playerGridPos = levelGrid.WorldToGrid(pos);
    //levelGrid.DrawCurrentCell(playerGridPos.x, playerGridPos.y);
}
#endif // DEBUG_RENDER

void PlayerInputComponent::SetUpKeyboardControls(unsigned long idx)
{
    auto& inputManager = dae::InputManager::GetInstance();
    if (idx == 0)
    {
        inputManager.AddKeyboardCommand(SDLK_UP, m_pMoveCommandUp.get());
        inputManager.AddKeyboardCommand(SDLK_DOWN, m_pMoveCommandDown.get());
        inputManager.AddKeyboardCommand(SDLK_LEFT, m_pMoveCommandLeft.get());
        inputManager.AddKeyboardCommand(SDLK_RIGHT, m_pMoveCommandRight.get());
    }
    if (idx == 1)
    {
        inputManager.AddKeyboardCommand(SDLK_w, m_pMoveCommandUp.get());
        inputManager.AddKeyboardCommand(SDLK_s, m_pMoveCommandDown.get());
        inputManager.AddKeyboardCommand(SDLK_a, m_pMoveCommandLeft.get());
        inputManager.AddKeyboardCommand(SDLK_d, m_pMoveCommandRight.get());
        inputManager.AddKeyboardCommand(SDLK_c, m_pSelfDamageCommand.get());
        inputManager.AddKeyboardCommand(SDLK_z, m_pAdd100PointsCommand.get());
        inputManager.AddKeyboardCommand(SDLK_q, m_pAdd10PointsCommand.get());
    }
}

/// GridMoveCommand.cpp
GridMoveCommand::GridMoveCommand(dae::GameObject& pGameObject,
                                 glm::i8vec2 m_Direction,
                                 int speed) :
    GameObjectCommand{ &pGameObject },
    m_Direction{ m_Direction },
    m_Speed{ speed },
    m_pPhysics{ GetGameObject()->GetComponent<dae::PhysicsComponent>() },
    m_pCollider{ GetGameObject()->GetComponent<dae::ColliderComponent>() }
{
    assert(m_pPhysics and m_pCollider and
           "GridMoveCommand requires a ColliderComponent/PhysicsComponent "
           "to function");
}

void GridMoveCommand::Execute()
{
    auto& levelGrid = LevelGrid::GetInstance();
    auto playerPos = GetGameObject()->GetWorldPosition();
    auto playerSize = m_pCollider->GetSize();

    glm::vec2 worldPos =
        playerPos + glm::vec2{ playerSize.x / 2.f, playerSize.y };
    glm::ivec2 gridPos = levelGrid.WorldToGrid(worldPos);
    glm::ivec2 targetGridPos =
        gridPos + glm::ivec2(m_Direction.x, m_Direction.y);

    bool canMove{};

    if (IsPositionedForWalking(worldPos, gridPos, targetGridPos) or
        IsPositionedForClimbing(worldPos, gridPos, targetGridPos))
        canMove = true;

    if (canMove)
    {
        glm::vec2 currentPos = GetGameObject()->GetWorldPosition();

        if (m_Direction.x != 0)
            currentPos.y = levelGrid.GridToWorld(targetGridPos).y -
                           m_pCollider->GetSize().y;
        else if (m_Direction.y != 0)
            currentPos.x = levelGrid.GridToWorld(targetGridPos).x -
                           (m_pCollider->GetSize().x / 2.f);
        float deltaTime = dae::EngineTime::GetInstance().GetDeltaTime();
        glm::vec2 newPos =
            currentPos +
            glm::vec2(static_cast<float>(m_Direction.x * m_Speed) * deltaTime,
                      static_cast<float>(m_Direction.y * m_Speed) * deltaTime);
        GetGameObject()->SetLocalPosition(newPos);
    }
}

void GridMoveCommand::Undo()
{
    if (auto* physics = GetGameObject()->GetComponent<dae::PhysicsComponent>())
    {
        auto velocity = physics->GetVelocity();
        if (m_Direction.x != 0)
            velocity.x = 0;
        if (m_Direction.y != 0)
            velocity.y = 0;
        physics->SetVelocity(velocity);
    }
}

bool GridMoveCommand::IsPositionedForClimbing(const glm::vec2& playerPos,
                                              const glm::ivec2& playerGridPos,
                                              glm::ivec2& targetGridPos)
{
    auto& levelGrid = LevelGrid::GetInstance();

    if (m_Direction.y == 0)
        return false;

    // Adjust target grid position based on horizontal alignment
    glm::vec2 cellWorldPos = levelGrid.GridToWorld(playerGridPos);
    float xOffset = playerPos.x - cellWorldPos.x;
    if (std::abs(xOffset - levelGrid.GRID_SIZE) < BUFFER)
        targetGridPos.x += 1;
    else if (std::abs(xOffset) > BUFFER)
        return false;

    auto leftCell = levelGrid.GetCellType(playerGridPos.x - 1, playerGridPos.y);
    auto rightCell = levelGrid.GetCellType(playerGridPos.x, playerGridPos.y);
    auto leftTargetCell =
        levelGrid.GetCellType(targetGridPos.x - 1, targetGridPos.y);
    auto rightTargetCell =
        levelGrid.GetCellType(targetGridPos.x, targetGridPos.y);

#ifdef DEBUG_RENDER
    dae::DebugRenderer::GetInstance().RenderRect(
        levelGrid.GridToWorld({ playerGridPos.x - 1, playerGridPos.y }),
        glm::vec2{ levelGrid.GRID_SIZE, levelGrid.GRID_SIZE });

    dae::DebugRenderer::GetInstance().RenderRect(
        levelGrid.GridToWorld({ playerGridPos.x, playerGridPos.y }),
        glm::vec2{ levelGrid.GRID_SIZE, levelGrid.GRID_SIZE });

    dae::DebugRenderer::GetInstance().RenderRect(
        levelGrid.GridToWorld({ targetGridPos.x - 1, targetGridPos.y }),
        glm::vec2{ levelGrid.GRID_SIZE, levelGrid.GRID_SIZE },
        { 1, 0, 0, 1 });

    dae::DebugRenderer::GetInstance().RenderRect(
        levelGrid.GridToWorld({ targetGridPos.x, targetGridPos.y }),
        glm::vec2{ levelGrid.GRID_SIZE, levelGrid.GRID_SIZE },
        { 1, 0, 0, 1 });
#endif // DEBUG_RENDER

    bool isLeftStair = IsLadder(leftCell);
    bool isRightStair = IsLadder(rightCell);

    bool isLeftTargetStair = IsLadder(leftTargetCell);
    bool isRightTargetStair = IsLadder(rightTargetCell);

    return { (isLeftStair and isRightStair) or
             (isLeftTargetStair and isRightTargetStair) };
}

bool GridMoveCommand::IsPositionedForWalking(const glm::vec2& playerPos,
                                             const glm::ivec2& playerGridPos,
                                             glm::ivec2& targetGridPos)
{
    auto& levelGrid = LevelGrid::GetInstance();

    if (m_Direction.x == 0)
        return false;

    // Adjust target grid position on player position
    float yOffset = playerPos.y - levelGrid.GridToWorld(playerGridPos).y;
    if (std::abs(yOffset - levelGrid.GRID_SIZE) < BUFFER)
        targetGridPos.y += 1;
    else if (std::abs(yOffset) > BUFFER)
        return false;

    auto targetCell = levelGrid.GetCellType(targetGridPos.x, targetGridPos.y);

#ifdef DEBUG_RENDER
    dae::DebugRenderer::GetInstance().RenderRect(
        levelGrid.GridToWorld({ playerGridPos.x, playerGridPos.y }),
        glm::vec2{ levelGrid.GRID_SIZE, levelGrid.GRID_SIZE });
    dae::DebugRenderer::GetInstance().RenderRect(
        levelGrid.GridToWorld({ targetGridPos.x, targetGridPos.y }),
        glm::vec2{ levelGrid.GRID_SIZE, levelGrid.GRID_SIZE },
        { 1, 0, 0, 1 });
#endif // DEBUG_RENDER

    return IsFloor(targetCell);
}

bool GridMoveCommand::IsLadder(CellType cell)
{
    return (cell == CellType::Ladder or cell == CellType::FloorAndLadder);
}

bool GridMoveCommand::IsFloor(CellType cell)
{
    return (cell == CellType::Floor or cell == CellType::FloorAndLadder);
}

AddPointsCommand::AddPointsCommand(dae::GameObject& pGameObject,
                                   int pointsAdded) :
    GameObjectCommand{ &pGameObject },
    m_pScoreComp{ pGameObject.GetComponent<dae::ScoreComponent>() },
    m_PointsAdded{ pointsAdded }
{}

void AddPointsCommand::Execute() { m_pScoreComp->AddScore(m_PointsAdded); }

SelfDamageCommand::SelfDamageCommand(dae::GameObject& pGameObject) :
    dae::GameObjectCommand{ &pGameObject },
    m_pLivesComp{ pGameObject.GetComponent<dae::LivesComponent>() }
{}

void SelfDamageCommand::Execute() { m_pLivesComp->LoseLife(); }
