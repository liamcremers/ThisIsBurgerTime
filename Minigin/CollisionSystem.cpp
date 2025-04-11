#include "CollisionSystem.h"
#include "ColliderComponent.h"
#include "DebugRenderer.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "BaseComponent.h"

void dae::CollisionSystem::RegisterCollider(dae::ColliderComponent* collider)
{
    if (collider)
    {
        m_Colliders.push_back(collider);
    }
}

void dae::CollisionSystem::ProcessCollisions()
{
    for (size_t i = 0; i < m_Colliders.size(); ++i)
    {
        for (size_t j = i + 1; j < m_Colliders.size(); ++j)
        {
            auto* a = m_Colliders[i];
            auto* b = m_Colliders[j];

            if ((a->GetMask() & b->GetLayer()) && a->CheckCollision(*b))
            {
                ResolveCollision(a, b);
            }
        }
    }
}

void dae::CollisionSystem::RenderColliders() const
{
    for (const auto& collider : m_Colliders)
    {
        DebugRenderer::GetInstance().RenderRect(collider->GetWorldPosition() +
                                                    collider->GetOffset(),
                                                collider->GetSize());
    }
}

void dae::CollisionSystem::ResolveCollision(ColliderComponent* a,
                                            ColliderComponent* b) const
{
    if (!a || !b)
        return;
    if (a->IsStatic() && b->IsStatic())
        return;

    a->OnCollision(*b);
    b->OnCollision(*a);
}
