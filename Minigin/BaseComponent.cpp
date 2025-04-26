#include "BaseComponent.h"

dae::BaseComponent::BaseComponent(GameObject& owner) :
    m_pOwner(owner)
{}

[[nodiscard]]
auto dae::BaseComponent::GetOwner() const -> GameObject&
{
    return m_pOwner;
}
