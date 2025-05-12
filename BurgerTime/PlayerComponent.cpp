#include "PlayerComponent.h"
#include "PlayerState.h"
#include "PlayerInputComponent.h"
#include "GridMoveCommand.h"
#include "LevelGrid.h"

#include <EngineTime.h>

PlayerComponent::PlayerComponent(dae::GameObject& parent) :
    BaseComponent{ parent },
    m_pCurrentState{ std::make_unique<IdleState>() },
    m_pInputComponent{ parent.GetComponent<PlayerInputComponent>() }
{
    assert(m_pInputComponent && "PlayerComponent requires a "
                                "PlayerInputComponent to function properly.");

    m_pInputComponent->GetMoveCommandDown()
        ->GetMoveCommandSubject()
        .AddObserver(this);
    m_pInputComponent->GetMoveCommandLeft()
        ->GetMoveCommandSubject()
        .AddObserver(this);
    m_pInputComponent->GetMoveCommandRight()
        ->GetMoveCommandSubject()
        .AddObserver(this);
    /**/ m_pInputComponent->GetMoveCommandUp()
        ->GetMoveCommandSubject()
        .AddObserver(this);
}

PlayerComponent::~PlayerComponent()
{
    if (m_pInputComponent)
    {
        m_pInputComponent->GetMoveCommandDown()
            ->GetMoveCommandSubject()
            .RemoveObserver(this);
        m_pInputComponent->GetMoveCommandLeft()
            ->GetMoveCommandSubject()
            .RemoveObserver(this);
        m_pInputComponent->GetMoveCommandRight()
            ->GetMoveCommandSubject()
            .RemoveObserver(this);
        m_pInputComponent->GetMoveCommandUp()
            ->GetMoveCommandSubject()
            .RemoveObserver(this);
    }
}

void PlayerComponent::Update()
{
    m_IdleTime += dae::EngineTime::GetInstance().GetDeltaTime();
    m_pCurrentState->Update(*this);
}

void PlayerComponent::LateUpdate()
{
    m_IsClimbing = false;
    m_IsWalking = false;
}

void PlayerComponent::ChangeState(std::unique_ptr<PlayerState> pNewState)
{
    if (m_pCurrentState)
        m_pCurrentState->Exit(*this);
    m_pCurrentState = std::move(pNewState);
    m_pCurrentState->Enter(*this);

#ifdef DEBUG_STATES
#include <typeinfo>
    dae::DebugRenderer::GetInstance().RenderText(
        "Loading state: " + std::string(typeid(*m_pCurrentState).name()));
#endif // DEBUG_STATES
}

void PlayerComponent::SetAnimation(const std::string&) {}

void PlayerComponent::SetCanMove(bool canMove)
{
    m_pInputComponent->SetCanMove(canMove);
}

bool PlayerComponent::IsMoving() const
{
    return m_IdleTime < m_IdleTimeThreshold;
}

bool PlayerComponent::IsWalking() const { return m_IsWalking; }

bool PlayerComponent::IsClimbing() const { return m_IsClimbing; }

void PlayerComponent::OnNotify(const std::string& eventId)
{
    if (eventId == "Walked")
        m_IsWalking = true;
    else if (eventId == "Climbed")
        m_IsClimbing = true;
    else
        return;
    m_IdleTime = 0.f;
}

void PlayerComponent::OnDestroy() { m_pInputComponent = nullptr; }
