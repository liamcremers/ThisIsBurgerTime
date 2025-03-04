#include "TransformComponent.h"

[[nodiscard]] const glm::vec2& dae::TransformComponent::GetWorldPosition() const
{
    return m_WorldPosition;
}

void dae::TransformComponent::SetWorldPosition(const glm::vec2& pos)
{
    m_WorldPosition = pos;
}

[[nodiscard]] const glm::vec2& dae::TransformComponent::GetLocalPosition() const
{
    return m_LocalPosition;
}

void dae::TransformComponent::SetLocalPosition(const glm::vec2& pos)
{
    m_LocalPosition = pos;
}
