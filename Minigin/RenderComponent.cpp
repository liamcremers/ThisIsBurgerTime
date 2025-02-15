#include "RenderComponent.h"

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::RenderComponent::Render() const
{
	if (m_texture == nullptr)
		return;

	const auto& pos{ this->GetGameObject()->GetPosition() };
	Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
}
