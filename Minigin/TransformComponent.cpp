#include "TransformComponent.h"

[[nodiscard]] auto dae::TransformComponent::GetWorldPosition() const
    -> const glm::vec2&
{
    return m_WorldPosition;
}

void dae::TransformComponent::SetWorldPosition(const glm::vec2& pos)
{
    m_WorldPosition = pos;
}

[[nodiscard]] auto dae::TransformComponent::GetLocalPosition() const
    -> const glm::vec2&
{
    return m_LocalPosition;
}

void dae::TransformComponent::SetLocalPosition(const glm::vec2& pos)
{
    m_LocalPosition = { std::roundf(pos.x), std::roundf(pos.y) };
}
