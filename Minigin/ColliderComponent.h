#pragma once
#include <glm.hpp>
#include "BaseComponent.h"

namespace dae
{
    class ColliderComponent : public BaseComponent
    {
    public:
        ColliderComponent(GameObject& parent, const glm::vec2& size = {});
        void OnCollision(const ColliderComponent& other) const;

        void SetSize(const glm::vec2& size);
        void SetOffset(const glm::vec2& offset);
        void SetCollisionMask(uint16_t mask);
        void SetCollisionLayer(uint16_t layer);
        void SetIsStatic(bool isStatic);

        [[nodiscard]] glm::vec2 GetWorldPosition() const;
        [[nodiscard]] glm::vec2 GetSize() const;
        [[nodiscard]] glm::vec2 GetOffset() const;
        [[nodiscard]] uint16_t GetMask();
        [[nodiscard]] uint16_t GetLayer();
        [[nodiscard]] bool CheckCollision(const ColliderComponent& other) const;
        [[nodiscard]] bool IsCollidingWith(const ColliderComponent&) const;
        [[nodiscard]] bool IsStatic() const;

    private:
        //struct CollisionData
        //{
        //    glm::vec2 normal; // Collision direction
        //    float penetration; // Overlap depth
        //};

        glm::vec2 m_Size{};
        uint16_t m_Layer = 0x0001;
        uint16_t m_Mask = 0xFFFF;
        bool m_IsStatic = false;
        glm::vec2 m_Offset{};
    };
}