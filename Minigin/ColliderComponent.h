#pragma once
#include "glm.hpp"
#include "BaseComponent.h"

#include <vector>
#include <functional>
#include <unordered_set>

enum class CollisionType
{
    Static,
    Dynamic,
    Trigger
};

namespace dae
{
    class ColliderComponent : public BaseComponent
    {
        using OverlapEvent =
            std::function<void(const ColliderComponent& other)>;

    public:
        ColliderComponent(GameObject& parent, const glm::vec2& size = {});
        void OnCollision(const ColliderComponent& other) const;

        void SubscribeToBeginOverlap(const OverlapEvent& callback);
        void SubscribeToEndOverlap(const OverlapEvent& callback);

        void AddOverlap(ColliderComponent* other);

        void RemoveOverlap(ColliderComponent* other)
        {
            m_CurrentOverlaps.erase(other);
        }

        bool IsOverlapping(ColliderComponent* other) const
        {
            return m_CurrentOverlaps.find(other) != m_CurrentOverlaps.end();
        }

        void SetCollisionType(CollisionType type) { m_CollisionType = type; }

        void SetHasMoved(bool hasMoved) { m_HasMoved = hasMoved; }

        void SetSize(const glm::vec2& size);
        void SetOffset(const glm::vec2& offset);
        void SetCollisionMask(uint16_t mask);
        void SetCollisionLayer(uint16_t layer);

        [[nodiscard]] glm::vec2 GetWorldPosition() const;
        [[nodiscard]] glm::vec2 GetSize() const;
        [[nodiscard]] glm::vec2 GetOffset() const;
        [[nodiscard]] uint16_t GetMask() const;
        [[nodiscard]] uint16_t GetLayer() const;
        [[nodiscard]] bool IsColliding(const ColliderComponent& other) const;
        [[nodiscard]] bool IsStatic() const;

        [[nodiscard]] bool HasMoved() const { return m_HasMoved; };

    private:
        friend class CollisionSystem;
        std::vector<OverlapEvent> m_BeginOverlapCallbacks;
        std::vector<OverlapEvent> m_EndOverlapCallbacks;

        std::unordered_set<ColliderComponent*> m_CurrentOverlaps;
        glm::vec2 m_Size{};
        glm::vec2 m_Offset{};
        uint16_t m_Layer = 0x0001;
        uint16_t m_Mask = 0xFFFF;
        CollisionType m_CollisionType = CollisionType::Dynamic;
        bool m_HasMoved{ false };
    };
}