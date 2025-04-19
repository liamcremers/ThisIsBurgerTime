#pragma once
#include "Singleton.h"

#include <vector>
#include <functional>
#include <set>
#include <algorithm>

namespace dae
{
    class ColliderComponent;

    class CollisionSystem : public Singleton<CollisionSystem>
    {
    public:
        void RegisterCollider(ColliderComponent* collider);

        void ProcessCollisions();

        [[nodiscard]] const std::vector<ColliderComponent*>&
        GetStaticColliders() const;

#ifdef DEBUG_RENDER
        void RenderColliders() const;
#endif // DEBUG_RENDER

    private:
        friend class Singleton<CollisionSystem>;
        CollisionSystem() = default;

        void ResolveCollision(ColliderComponent* a, ColliderComponent* b) const;
        bool ShouldCollide(const ColliderComponent& a,
                           const ColliderComponent& b) const;
        void HandleCollision(ColliderComponent* a, ColliderComponent* b);
        void CleanupEndedOverlaps();

        std::vector<ColliderComponent*> m_StaticColliders;
        std::vector<ColliderComponent*> m_DynamicColliders;

        struct CollisionPair
        {
            ColliderComponent* a;
            ColliderComponent* b;

            bool operator<(const CollisionPair& other) const
            {
                auto [min1, max1] = std::minmax(
                    a, b, [](auto* x, auto* y) { return std::less<>{}(x, y); });
                auto [min2, max2] = std::minmax(
                    other.a,
                    other.b,
                    [](auto* x, auto* y) { return std::less<>{}(x, y); });
                return std::tie(min1, max1) < std::tie(min2, max2);
            }
        };

        std::set<CollisionPair> m_PrevCollisions;
    };
}
