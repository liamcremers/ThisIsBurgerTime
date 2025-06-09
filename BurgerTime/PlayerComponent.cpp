#include "PlayerComponent.h"
#include "PlayerInputComponent.h"
#include "BurgerTimeLayers.h"

#ifdef DEBUG_STATES
#include <DebugRenderer.h>
#endif // DEBUG_STATES

PlayerComponent::PlayerComponent(dae::GameObject& parent) :
    BaseComponent{ parent }
{
    SetupStateTextures();
    CreateOverlapEvent(parent);
}

void PlayerComponent::CreateOverlapEvent(dae::GameObject& parent)
{
    if (auto* collider = parent.GetComponent<dae::ColliderComponent>())
    {
        collider->SubscribeToBeginOverlap(
            [this](const dae::ColliderComponent& other)
            {
                if ((other.GetLayer() &
                     static_cast<uint16_t>(CollisionLayer::Enemy)) != 0)
                    OnHitByEnemy();
            });
    }
}

void PlayerComponent::SetupStateTextures()
{
    LoadStateTexture(
        &m_IdleState, DirectionVec::Down, "ChefPeterPepperIdleF.png");
    LoadStateTexture(
        &m_IdleState, DirectionVec::Left, "ChefPeterPepperIdleF.png");
    LoadStateTexture(
        &m_IdleState, DirectionVec::Right, "ChefPeterPepperIdleF.png");
    LoadStateTexture(
        &m_IdleState, DirectionVec::Up, "ChefPeterPepperIdleB.png");

    LoadStateTexture(&m_MoveState, DirectionVec::Down, "ChefPeterPepperF.png");
    LoadStateTexture(&m_MoveState, DirectionVec::Left, "ChefPeterPepperL.png");
    LoadStateTexture(&m_MoveState, DirectionVec::Right, "ChefPeterPepperR.png");
    LoadStateTexture(&m_MoveState, DirectionVec::Up, "ChefPeterPepperB.png");

    //TODO FIX
    LoadStateTexture(
        &m_AttackState, DirectionVec::Down, "ChefPeterPepperIdleF.png");
    LoadStateTexture(
        &m_AttackState, DirectionVec::Left, "ChefPeterPepperIdleF.png");
    LoadStateTexture(
        &m_AttackState, DirectionVec::Right, "ChefPeterPepperIdleF.png");
    LoadStateTexture(
        &m_AttackState, DirectionVec::Up, "ChefPeterPepperIdleB.png");

    LoadStateTexture(
        &m_DieState, DirectionVec::Down, "ChefPeterPepperDead.png");
    LoadStateTexture(
        &m_DieState, DirectionVec::Left, "ChefPeterPepperDead.png");
    LoadStateTexture(
        &m_DieState, DirectionVec::Right, "ChefPeterPepperDead.png");
    LoadStateTexture(&m_DieState, DirectionVec::Up, "ChefPeterPepperDead.png");

    m_pSpriteComponent->SetTexture(m_TexturePath[m_pCurrentState][m_Direction]);
}

void PlayerComponent::Update()
{
    auto& newState = m_pCurrentState->Update(*this);
    if (&newState != m_pCurrentState)
    {
        ChangeState(&newState);
        UpdateSprite();
    }
    UpdateTimers();
}

void PlayerComponent::UpdateTimers()
{
    m_TimeSinceMoved += dae::EngineTime::GetInstance().GetDeltaTime();
}

auto PlayerComponent::DirectionToEnum(glm::vec2 dir) -> Direction
{
    if (dir == DirectionVec::Left)
        return Direction::Left;
    if (dir == DirectionVec::Up)
        return Direction::Up;
    if (dir == DirectionVec::Right)
        return Direction::Right;
    if (dir == DirectionVec::Down)
        return Direction::Down;
    return Direction::Down;
}

void PlayerComponent::HandleInput(PlayerInputKeys input)
{
    auto& newState = m_pCurrentState->HandleInput(*this, input);
    if (&newState != m_pCurrentState)
    {
        ChangeState(&newState);
        UpdateSprite();
    }
}

auto PlayerComponent::GetIdleState() -> IdleState& { return m_IdleState; }

auto PlayerComponent::GetMoveState() -> MoveState& { return m_MoveState; }

auto PlayerComponent::GetAttackState() -> AttackState& { return m_AttackState; }

auto PlayerComponent::GetDieState() -> DieState& { return m_DieState; }

void PlayerComponent::OnMove(glm::vec2 direction)
{
    bool didExecute{};

    if (direction == DirectionVec::Left)
        didExecute = m_MoveCommandLeft->TryExecute();
    else if (direction == DirectionVec::Up)
        didExecute = m_MoveCommandUp->TryExecute();
    else if (direction == DirectionVec::Right)
        didExecute = m_MoveCommandRight->TryExecute();
    else if (direction == DirectionVec::Down)
        didExecute = m_MoveCommandDown->TryExecute();

    if (didExecute)
    {
        auto oldDir = m_Direction;
        SetSpriteDirection(direction);
        if (oldDir != m_Direction)
            UpdateSprite();
        m_TimeSinceMoved = 0.f;
    }
}

void PlayerComponent::OnHitByEnemy()
{
    ChangeState(&m_DieState);
    UpdateSprite();
}

void PlayerComponent::OnDeath()
{
    GetOwner().GetComponent<dae::LivesComponent>()->LoseLife();
}

auto PlayerComponent::HasMoved() const -> bool
{
    return m_TimeSinceMoved < MOVED_BUFFER;
}

void PlayerComponent::UpdateSprite()
{
    m_pSpriteComponent->Reset();
    m_pSpriteComponent->SetTexture(m_TexturePath[m_pCurrentState][m_Direction]);
}

void PlayerComponent::SetSpriteDirection(glm::vec2 directionVec)
{
    m_Direction = DirectionToEnum(directionVec);
}

void PlayerComponent::LoadStateTexture(PlayerState* stateT,
                                       glm::vec2 directionVec,
                                       const std::string& texturePath)
{
    auto direction = DirectionToEnum(directionVec);
    m_TexturePath[stateT][direction] = texturePath;
}

void PlayerComponent::ChangeState(PlayerState* const state)
{
    if (m_pCurrentState)
        m_pCurrentState->Exit(*this);
    m_pCurrentState = state;
    m_pCurrentState->Enter(*this);

#ifdef DEBUG_STATES
#include <typeinfo>
    dae::DebugRenderer::GetInstance().RenderText(
        "Loading state: " + std::string(typeid(*m_pCurrentState).name()));
#endif // DEBUG_STATES
}
