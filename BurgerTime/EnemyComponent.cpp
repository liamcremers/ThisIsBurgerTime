#include "EnemyComponent.h"
#include "EnemyInputComponent.h"
#include "BurgerTimeLayers.h"
#include "LivesComponent.h"

#ifdef DEBUG_STATES
#include <DebugRenderer.h>
#endif // DEBUG_STATES
static dae::Subject s_DiedSubject{};

EnemyComponent::EnemyComponent(dae::GameObject& parent) :
    BaseComponent{ parent }
{
    SetupStateTextures();
    CreateOverlapEvent(parent);
}

void EnemyComponent::CreateOverlapEvent(dae::GameObject& parent)
{
    if (auto* collider = parent.GetComponent<dae::ColliderComponent>())
    {
        collider->SetCollisionType(CollisionType::Trigger);
        collider->SubscribeToBeginOverlap(
            [this](const dae::ColliderComponent& other)
            {
                if (IsLayerType(other, CollisionLayer::BurgerPlate))
                {
                    ChangeState(&m_DieState);
                    UpdateSprite();
                }
            });
    }
}

void EnemyComponent::SetupStateTextures()
{
    LoadStateTexture(&m_IdleState, DirectionVec::None, "MrHotDog.png");

    LoadStateTexture(&m_MoveState, DirectionVec::Down, "MrHotDogF.png");
    LoadStateTexture(&m_MoveState, DirectionVec::Left, "MrHotDogL.png");
    LoadStateTexture(&m_MoveState, DirectionVec::Right, "MrHotDogR.png");
    LoadStateTexture(&m_MoveState, DirectionVec::Up, "MrHotDogB.png");

    LoadStateTexture(&m_AttackState, DirectionVec::None, "MrHotDog.png");

    LoadStateTexture(&m_DieState, DirectionVec::None, "MrHotDogCrushed.png");

    UpdateSprite();
}

void EnemyComponent::Update()
{
    auto& newState = m_pCurrentState->Update(*this);
    if (&newState != m_pCurrentState)
    {
        ChangeState(&newState);
        UpdateSprite();
    }
    UpdateTimers();
}

void EnemyComponent::UpdateTimers()
{
    m_TimeSinceMoved += dae::EngineTime::GetInstance().GetDeltaTime();
}

auto EnemyComponent::DirectionToEnum(glm::vec2 dir) -> Direction
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

void EnemyComponent::HandleInput(EnemyInputKeys input)
{
    auto& newState = m_pCurrentState->HandleInput(*this, input);
    if (&newState != m_pCurrentState)
    {
        ChangeState(&newState);
        UpdateSprite();
    }
}

dae::Subject& EnemyComp::EnemyComponent::GetStaticDiedSubject()
{
    return s_DiedSubject;
}

auto EnemyComponent::GetIdleState() -> IdleState& { return m_IdleState; }

auto EnemyComponent::GetMoveState() -> MoveState& { return m_MoveState; }

auto EnemyComponent::GetAttackState() -> AttackState& { return m_AttackState; }

auto EnemyComponent::GetDieState() -> DieState& { return m_DieState; }

void EnemyComponent::OnMove(glm::vec2 direction)
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

void EnemyComp::EnemyComponent::OnDieByBurger()
{
    ChangeState(&m_DieState);
    UpdateSprite();
}

void EnemyComp::EnemyComponent::OnMoveWithBurger(dae::GameObject& burger)
{
    GetOwner().SetParent(&burger, true);
    ChangeState(&m_IdleState);
    UpdateSprite();
}

void EnemyComponent::OnDeath()
{
    s_DiedSubject.Notify("HotDogDied", GetOwner().GetWorldPosition());
    GetOwner().GetComponent<LivesComponent>()->LoseLife();
}

auto EnemyComponent::HasMoved() const -> bool
{
    return m_TimeSinceMoved < MOVED_BUFFER;
}

void EnemyComponent::UpdateSprite()
{
    m_pSpriteComponent->Reset();
    if (m_pCurrentState == &m_MoveState)
        m_pSpriteComponent->SetTexture(
            m_TexturePath[m_pCurrentState][m_Direction]);
    else
        m_pSpriteComponent->SetTexture(
            m_TexturePath[m_pCurrentState][Direction::None]);
}

void EnemyComponent::SetSpriteDirection(glm::vec2 directionVec)
{
    m_Direction = DirectionToEnum(directionVec);
}

void EnemyComponent::LoadStateTexture(EnemyState* stateT,
                                      glm::vec2 directionVec,
                                      const std::string& texturePath)
{
    auto direction = (stateT == &GetMoveState()) ?
                         DirectionToEnum(directionVec) :
                         Direction::None;
    m_TexturePath[stateT][direction] = texturePath;
}

void EnemyComponent::ChangeState(EnemyState* const state)
{
    if (m_pCurrentState)
        m_pCurrentState->Exit(*this);
    m_pCurrentState = state;
    m_pCurrentState->Enter(*this);

#ifdef DEBUG_STATES
#include <typeinfo>
    dae::DebugRenderer::GetInstance().RenderText(
        "Enemy state: " + std::string(typeid(*m_pCurrentState).name()));
#endif // DEBUG_STATES
}
