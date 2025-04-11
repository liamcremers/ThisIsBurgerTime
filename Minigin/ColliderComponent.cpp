#include "ColliderComponent.h"
#include "GameObject.h"

dae::ColliderComponent::ColliderComponent(GameObject& parent,
                                          const glm::vec2& size) :
    BaseComponent(parent),
    m_Size(size),
    m_IsStatic{ parent.GetComponent<PhysicsComponent>() == nullptr }
{}

void dae::ColliderComponent::OnCollision(const ColliderComponent& other) const
{
    if (m_IsStatic && other.m_IsStatic)
        return;

    // Determine which collider is dynamic
    ColliderComponent* dynamicCollider =
        m_IsStatic ? const_cast<ColliderComponent*>(&other) :
                     const_cast<ColliderComponent*>(this);
    const ColliderComponent* staticCollider = m_IsStatic ? this : &other;

    // Calculate overlap
    const glm::vec2 staticPos =
        staticCollider->GetWorldPosition() + staticCollider->m_Offset;
    const glm::vec2 dynamicPos =
        dynamicCollider->GetWorldPosition() + dynamicCollider->m_Offset;

    // Calculate penetration in both axes
    const float overlapX =
        (dynamicPos[0] < staticPos[0]) ?
            (dynamicPos[0] + dynamicCollider->m_Size[0]) - staticPos[0] :
            (staticPos[0] + staticCollider->m_Size[0]) - dynamicPos[0];

    const float overlapY =
        (dynamicPos[1] < staticPos[1]) ?
            (dynamicPos[1] + dynamicCollider->m_Size[1]) - staticPos[1] :
            (staticPos[1] + staticCollider->m_Size[1]) - dynamicPos[1];

    // Resolve along smallest overlap axis
    if (abs(overlapX) < abs(overlapY))
    {
        // X-axis resolution
        glm::vec2 newPos = dynamicCollider->GetOwner().GetLocalPosition();
        newPos[0] += (dynamicPos[0] < staticPos[0]) ? -overlapX : overlapX;
        dynamicCollider->GetOwner().SetLocalPosition(newPos);

        // Stop horizontal movement
        if (auto* physics =
                dynamicCollider->GetOwner().GetComponent<PhysicsComponent>())
        {
            physics->SetVelocity({ 0, physics->GetVelocity()[1] });
        }
    }
    else
    {
        // Y-axis resolution
        glm::vec2 newPos = dynamicCollider->GetOwner().GetLocalPosition();
        newPos[1] += (dynamicPos[1] < staticPos[1]) ? -overlapY : overlapY;
        dynamicCollider->GetOwner().SetLocalPosition(newPos);

        // Stop vertical movement
        if (auto* physics =
                dynamicCollider->GetOwner().GetComponent<PhysicsComponent>())
        {
            physics->SetVelocity({ physics->GetVelocity()[0], 0 });
        }
    }
}

void dae::ColliderComponent::SetSize(const glm::vec2& size) { m_Size = size; }

void dae::ColliderComponent::SetOffset(const glm::vec2& offset)
{
    m_Offset = offset;
}

void dae::ColliderComponent::SetCollisionMask(uint16_t mask) { m_Mask = mask; }

void dae::ColliderComponent::SetCollisionLayer(uint16_t layer)
{
    m_Layer = layer;
}

void dae::ColliderComponent::SetIsStatic(bool isStatic)
{
    m_IsStatic = isStatic;
}

[[nodiscard]] auto dae::ColliderComponent::GetWorldPosition() const -> glm::vec2
{
    return GetOwner().GetWorldPosition();
}

[[nodiscard]] auto dae::ColliderComponent::GetSize() const -> glm::vec2
{
    return m_Size;
}

[[nodiscard]] auto dae::ColliderComponent::GetOffset() const -> glm::vec2
{
    return m_Offset;
}

[[nodiscard]] auto dae::ColliderComponent::GetMask() -> uint16_t
{
    return m_Mask;
}

[[nodiscard]] auto dae::ColliderComponent::GetLayer() -> uint16_t
{
    return m_Layer;
}

[[nodiscard]] auto dae::ColliderComponent::CheckCollision(
    const ColliderComponent& other) const -> bool
{
    const glm::vec2 thisWorldPos = GetWorldPosition() + m_Offset;
    const glm::vec2 otherWorldPos = other.GetWorldPosition() + other.m_Offset;

    return (thisWorldPos[0] < otherWorldPos[0] + other.m_Size[0] &&
            thisWorldPos[0] + m_Size[0] > otherWorldPos[0] &&
            thisWorldPos[1] < otherWorldPos[1] + other.m_Size[1] &&
            thisWorldPos[1] + m_Size[1] > otherWorldPos[1]);
}

[[nodiscard]] auto dae::ColliderComponent::IsCollidingWith(
    const ColliderComponent& other) const -> bool
{
    return CheckCollision(other);
}

[[nodiscard]] auto dae::ColliderComponent::IsStatic() const -> bool
{
    return m_IsStatic;
}
