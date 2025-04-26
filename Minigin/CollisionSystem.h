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

        [[nodiscard]] auto GetStaticColliders() const
            -> const std::vector<ColliderComponent*>&;

#ifdef DEBUG_RENDER
        void RenderColliders() const;
#endif // DEBUG_RENDER

    private:
        friend class Singleton<CollisionSystem>;
        CollisionSystem() = default;

        void ResolveCollision(ColliderComponent* a, ColliderComponent* b) const;
        void HandleCollision(ColliderComponent* a, ColliderComponent* b);
        void CleanupEndedOverlaps();
        [[nodiscard]]
        auto ShouldCollide(const ColliderComponent& a,
                           const ColliderComponent& b) const -> bool;

        std::vector<ColliderComponent*> m_StaticColliders;
        std::vector<ColliderComponent*> m_DynamicColliders;

        struct CollisionPair
        {
            ColliderComponent* a;
            ColliderComponent* b;

            bool operator<(const CollisionPair& other) const;
        };

        std::set<CollisionPair> m_PrevCollisions;
    };
}
