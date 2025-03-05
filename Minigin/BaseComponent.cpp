#include "BaseComponent.h"

[[nodiscard]] auto dae::BaseComponent::GetOwner() const -> dae::GameObject&
{
    return m_pOwner;
}