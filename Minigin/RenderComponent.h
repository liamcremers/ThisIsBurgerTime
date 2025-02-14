#pragma once
#include "BaseComponent.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h" 

#include <iostream>
namespace dae
{
	class RenderComponent final : public BaseComponent
	{
	public:
		RenderComponent(GameObject* parent) :BaseComponent(parent) {}

		void SetTexture(const std::string& filename);
	private:
		std::shared_ptr<Texture2D> m_texture{};

		// Inherited via BaseComponent
		void Update() override;
		void Render() const override;
	};
}