#pragma once
#include "BaseComponent.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h" 

#include <iostream>
#include <glm.hpp>

namespace dae
{
	class TransformComponent final : public BaseComponent
	{
	public:
		TransformComponent(GameObject* parent) :BaseComponent(parent), m_Position{ 0.0f, 0.0f } {}
		TransformComponent(GameObject* parent, glm::vec2 pos) :BaseComponent(parent), m_Position{ pos } {}
		TransformComponent(GameObject* parent, const float x, const float y) :BaseComponent(parent), m_Position{ x, y } {}

		const glm::vec2& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec2& pos) { m_Position = pos; }
	private:
		glm::vec2 m_Position{};

		void Update() override {};
		void Render() const override {};
		void FixedUpdate() override {};
		void LateUpdate() override {};
	};
}
