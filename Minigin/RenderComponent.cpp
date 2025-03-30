#include "RenderComponent.h"
#include "GameObject.h"

dae::RenderComponent::RenderComponent(GameObject& parent) :
    BaseComponent(parent)
{}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
    m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::RenderComponent::Scale(float scale) const { m_texture->Scale(scale); }

void dae::RenderComponent::Render() const
{
    if (m_texture == nullptr)
        return;

    const auto& pos{ GetOwner().GetWorldPosition() };
    Renderer::GetInstance().RenderTexture(*m_texture, pos[0], pos[1]);
}
