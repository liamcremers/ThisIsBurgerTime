#include "PhysicsComponent.h"
#include "GameObject.h"
#include "EngineTime.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"

dae::PhysicsComponent::PhysicsComponent(GameObject& parent, bool useGravity) :
    BaseComponent{ parent },
    m_UseGravity{ useGravity },
    m_pCollider{ parent.GetComponent<ColliderComponent>() }
{
    assert(m_pCollider != nullptr &&
           "PhysicsComponent requires a ColliderComponent to function");
    m_pCollider->SetCollisionType(CollisionType::Dynamic);
}

void dae::PhysicsComponent::FixedUpdate()
{
    auto fixedDelta = EngineTime::GetInstance().GetFixedTimeStep();

    if (m_UseGravity)
    {
        m_Velocity[1] += m_Gravity * fixedDelta;
        m_Velocity[1] = std::min(m_Velocity[1], m_MaxFallSpeed);
    }
    if (m_Velocity != glm::vec2{ 0, 0 })
    {
        GetOwner().SetLocalPosition(GetOwner().GetLocalPosition() +
                                    m_Velocity * fixedDelta);

        m_pCollider->SetHasMoved(true);
    }
}

void dae::PhysicsComponent::SetVelocity(const glm::vec2& velocity)
{
    m_Velocity = velocity;
}

void dae::PhysicsComponent::SetGravity(float gravity) { m_Gravity = gravity; }

void dae::PhysicsComponent::SetMaxFallSpeed(float speed)
{
    m_MaxFallSpeed = speed;
}

void dae::PhysicsComponent::SetUseGravity(bool useGravity)
{
    m_UseGravity = useGravity;
}

[[nodiscard]] auto dae::PhysicsComponent::GetVelocity() const -> glm::vec2
{
    return m_Velocity;
}

[[nodiscard]] auto dae::PhysicsComponent::GetGravity() const -> float
{
    return m_Gravity;
}

[[nodiscard]] auto dae::PhysicsComponent::GetMaxFallSpeed() const -> float
{
    return m_MaxFallSpeed;
}

[[nodiscard]] auto dae::PhysicsComponent::IsUsingGravity() const -> bool
{
    return m_UseGravity;
}
