#pragma once
#include "PlayerComponent.h"
#include "PlayerInputComponent.h"

#ifdef DEBUG_STATES
#include <DebugRenderer.h>
#endif // DEBUG_STATES

PlayerComponent::PlayerComponent(dae::GameObject& parent) :
    BaseComponent{ parent }
{}

void PlayerComponent::Update()
{
    auto& newState = m_pCurrentState->Update(*this);
    if (&newState != m_pCurrentState)
        ChangeState(&newState);

    UpdateTimers();
}

void PlayerComponent::UpdateTimers()
{
    m_TimeSinceMoved += dae::EngineTime::GetInstance().GetDeltaTime();
}

void PlayerComponent::HandleInput(PlayerInputKeys input)
{
    auto& newState = m_pCurrentState->HandleInput(*this, input);
    if (&newState != m_pCurrentState)
        ChangeState(&newState);
}

IdleState& PlayerComponent::GetIdleState() { return m_IdleState; }

MoveState& PlayerComponent::GetMoveState() { return m_MoveState; }

AttackState& PlayerComponent::GetAttackState() { return m_AttackState; }

DieState& PlayerComponent::GetDieState() { return m_DieState; }

void PlayerComponent::Move(glm::vec2 direction)
{
    if (direction == PlayerDirection::Left)
        m_MoveCommandLeft->Execute();
    if (direction == PlayerDirection::Up)
        m_MoveCommandUp->Execute();
    if (direction == PlayerDirection::Right)
        m_MoveCommandRight->Execute();
    if (direction == PlayerDirection::Down)
        m_MoveCommandDown->Execute();

    m_TimeSinceMoved = 0.f;
}

bool PlayerComponent::HasMoved() const
{
    return m_TimeSinceMoved < MOVED_BUFFER;
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
