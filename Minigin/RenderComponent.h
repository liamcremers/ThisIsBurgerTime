#pragma once
#include "BaseComponent.h"
#include "ResourceManager.h"
#include "Renderer.h" 

#include <iostream>
namespace dae
{
	class RenderComponent final : public BaseComponent
	{
	public:
		RenderComponent(GameObject& parent);

		void Render() const override;
		void SetTexture(const std::string& filename);
	private:
		Texture2D* m_texture{};
	};
}