#pragma once
#include "BaseComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"

#include <iostream>
#include <glm.hpp>

namespace dae
{
    class TransformComponent final : public BaseComponent
    {
    public:
        TransformComponent(GameObject& owner) :
            BaseComponent(owner),
            m_Position{ 0.0f, 0.0f }
        {}
        TransformComponent(GameObject& owner, glm::vec2 pos) :
            BaseComponent(owner),
            m_Position{ pos }
        {}
        TransformComponent(GameObject& owner, const float x, const float y) :
            BaseComponent(owner),
            m_Position{ x, y }
        {}

        [[nodiscard]] const glm::vec2& GetWorldPosition() const
        {
            return m_WorldPosition;
        };
        void SetWorldPosition(const glm::vec2& pos) { m_WorldPosition = pos; };
        [[nodiscard]] const glm::vec2& GetLocalPosition() const
        {
            return m_LocalPosition;
        }
        void SetLocalPosition(const glm::vec2& pos) { m_LocalPosition = pos; }

    private:
        glm::vec2 m_Position{};
        glm::vec2 m_WorldPosition{};
        glm::vec2 m_LocalPosition{};
    };
}
