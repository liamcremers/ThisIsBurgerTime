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
		void Render() const override;

		void SetTexture(const std::string& filename);
	private:
		std::shared_ptr<Texture2D> m_texture{};

		void Update() override {};
		void FixedUpdate() override {};
		void LateUpdate() override {};
	};
}