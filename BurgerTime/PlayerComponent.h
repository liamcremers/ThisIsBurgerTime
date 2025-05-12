#pragma once
#include "PlayerState.h"

#include <BaseComponent.h>
#include <GameObject.h>
#include <Command.h>
#include <Observer.h>
#include <glm.hpp>

#include <memory>
#include <vector>
#include <string>

class PlayerInputComponent;

class PlayerComponent final : public dae::BaseComponent, public dae::Observer
{
public:
    PlayerComponent(dae::GameObject& parent);
    virtual ~PlayerComponent();
    void Update() override;
    void LateUpdate() override;

    void ChangeState(std::unique_ptr<PlayerState> pNewState);
    void SetAnimation(const std::string& animationName);
    void SetCanMove(bool canMove);

    bool IsMoving() const;
    bool IsWalking() const;
    bool IsClimbing() const;

protected:
    void OnNotify(const std::string& eventId) override;
    void OnDestroy() override;

private:
    std::unique_ptr<PlayerState> m_pCurrentState;
    PlayerInputComponent* m_pInputComponent;
    dae::ColliderComponent* m_pCollider{
        GetOwner().GetComponent<dae::ColliderComponent>()
    };
    float m_IdleTime{ 0.f };
    const float m_IdleTimeThreshold{ 0.15f };

    bool m_IsWalking{ false };
    bool m_IsClimbing{ false };
};
