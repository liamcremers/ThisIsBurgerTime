#pragma once
#include "IControllable.h"
#include "Direction.h"
#include "LevelGrid.h"
#include <GameObject.h>
#include <BaseComponent.h>
#include <glm.hpp>
#include <vector>
#include <utility>

class AIControllerComponent;

namespace AIControllerStates
{
    class AIState
    {
    public:
        virtual ~AIState() = default;
        virtual AIState& Update(AIControllerComponent& pAIController) = 0;
    };

    class HorizontalTravelState : public AIState
    {
    public:
        AIState& Update(AIControllerComponent&) override;
    };

    class VerticalTravelState : public AIState
    {
    public:
        AIState& Update(AIControllerComponent&) override;
    };
}

class PlayerComponent;

class AIControllerComponent : public dae::BaseComponent
{
public:
    AIControllerComponent(dae::GameObject& owner,
                          IControllable* pControlled,
                          PlayerComponent* pPlayer);
    void Update();
    void MoveTowardTarget(glm::vec2 target);
    auto GetPosition() const -> glm::vec2;
    auto GetVerticalTravelState() -> AIControllerStates::VerticalTravelState&;
    auto GetHorizontalTravelState()
        -> AIControllerStates::HorizontalTravelState&;
    auto GetCurrentState() const -> AIControllerStates::AIState*;
    auto GetPlayerPosition() const -> glm::vec2;

private:
    auto GetDirectionToTarget(const glm::vec2& target,
                              const glm::vec2& position) -> Direction;

    AIControllerStates::HorizontalTravelState m_HorizontalTravelState{};
    AIControllerStates::VerticalTravelState m_VerticalTravelState{};
    AIControllerStates::AIState* m_pCurrentState{ &m_HorizontalTravelState };
    IControllable* m_pControlled{};
    PlayerComponent* m_pPlayer{};
};
