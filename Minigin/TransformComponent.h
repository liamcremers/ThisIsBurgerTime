#pragma once
#include "BaseComponent.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h" 

#include <iostream>
#include <glm.hpp>

namespace dae
{
	//TODO: rewrite as a component
	//maybe glm::vec2 can take two floats as parameters
	class TransformComponent final : public BaseComponent
	{
	public:
		TransformComponent(GameObject* parent) :BaseComponent(parent), m_Position{ 0.0f, 0.0f } {}
		TransformComponent(GameObject* parent, glm::vec2 pos) :BaseComponent(parent), m_Position{ pos } {}
		TransformComponent(GameObject* parent, const float x, const float y) :BaseComponent(parent), m_Position{ x, y } {}

		const glm::vec2& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec2& pos) { m_Position = pos; }
		void SetPosition(float x, float y) { m_Position = { x, y }; }
	private:
		glm::vec2 m_Position{};

		// Inherited via BaseComponent
		void Update() override;
		void Render() const override;
	};
}
