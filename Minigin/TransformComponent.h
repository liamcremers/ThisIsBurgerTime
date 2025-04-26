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
            BaseComponent(owner)
        {}

        [[nodiscard]] auto GetWorldPosition() const -> const glm::vec2&;
        void SetWorldPosition(const glm::vec2& pos);
        [[nodiscard]] auto GetLocalPosition() const -> const glm::vec2&;
        void SetLocalPosition(const glm::vec2& pos);

    private:
        glm::vec2 m_WorldPosition{};
        glm::vec2 m_LocalPosition{};
    };
}
