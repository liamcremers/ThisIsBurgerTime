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
        void RemoveOverlap(ColliderComponent* other);

        void SetCollisionType(CollisionType type);
        void SetHasMoved(bool hasMoved);
        void SetSize(const glm::vec2& size);
        void SetOffset(const glm::vec2& offset);
        void SetCollisionMask(uint16_t mask);
        void SetCollisionLayer(uint16_t layer);

        [[nodiscard]] auto GetWorldPosition() const -> glm::vec2;
        [[nodiscard]] auto GetSize() const -> glm::vec2;
        [[nodiscard]] auto GetOffset() const -> glm::vec2;
        [[nodiscard]] auto GetMask() const -> uint16_t;
        [[nodiscard]] auto GetLayer() const -> uint16_t;
        [[nodiscard]] auto IsStatic() const -> bool;
        [[nodiscard]] auto HasMoved() const -> bool;
        [[nodiscard]]
        auto IsOverlapping(ColliderComponent* other) const -> bool;
        [[nodiscard]]
        auto IsColliding(const ColliderComponent& other) const -> bool;

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