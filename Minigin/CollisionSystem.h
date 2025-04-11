#pragma once
#include "Singleton.h"
#include <vector>
#include <functional>

namespace dae
{
    class ColliderComponent;

    class CollisionSystem : public Singleton<CollisionSystem>
    {
    public:
        void RegisterCollider(ColliderComponent* collider);
        void ProcessCollisions();
        void RenderColliders() const;

    private:
        friend class Singleton<CollisionSystem>;
        CollisionSystem() = default;

        void ResolveCollision(ColliderComponent* a, ColliderComponent* b) const;
        std::vector<ColliderComponent*> m_Colliders;
    };
}
