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

        [[nodiscard]] const glm::vec2& GetWorldPosition() const;
        void SetWorldPosition(const glm::vec2& pos);
        [[nodiscard]] const glm::vec2& GetLocalPosition() const;
        void SetLocalPosition(const glm::vec2& pos);

    private:
        glm::vec2 m_WorldPosition{};
        glm::vec2 m_LocalPosition{};
    };
}
