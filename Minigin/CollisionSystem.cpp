#include "CollisionSystem.h"
#include "ColliderComponent.h"
#include "DebugRenderer.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "BaseComponent.h"
#include <algorithm>
#include <set>
#include <ranges>

void dae::CollisionSystem::RegisterCollider(dae::ColliderComponent* collider)
{
    if (collider->IsStatic())
        m_StaticColliders.push_back(collider);
    else
        m_DynamicColliders.push_back(collider);
}

void dae::CollisionSystem::ProcessCollisions()
{
    for (auto* dynamicCollider :
         m_DynamicColliders | std::views::filter(&ColliderComponent::HasMoved))
    {
        for (auto& ColliderList : { m_DynamicColliders, m_StaticColliders })
            for (auto& other : ColliderList)
            {
                if (dynamicCollider == other)
                    continue;
                if (ShouldCollide(*dynamicCollider, *other) &&
                    dynamicCollider->IsColliding(*other))
                {
                    HandleCollision(dynamicCollider, other);
                }
            }

        dynamicCollider->SetHasMoved(false);
    }

    CleanupEndedOverlaps();
}

#ifdef DEBUG_RENDER
void dae::CollisionSystem::RenderColliders() const
{
    for (const auto& colliderList : { m_DynamicColliders, m_StaticColliders })
    {
        for (const auto& collider : colliderList)
        {
            DebugRenderer::GetInstance().RenderRect(
                collider->GetWorldPosition() + collider->GetOffset(),
                collider->GetSize());
        }
    }
}
#endif // DEBUG_RENDER

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

bool dae::CollisionSystem::ShouldCollide(const ColliderComponent& a,
                                         const ColliderComponent& b) const
{
    return (a.GetMask() & b.GetLayer()) && (b.GetMask() & a.GetLayer());
}

void dae::CollisionSystem::HandleCollision(ColliderComponent* a,
                                           ColliderComponent* b)
{
    if (!a->IsOverlapping(b))
    {
        a->AddOverlap(b);
        b->AddOverlap(a);

        std::ranges::for_each(a->m_BeginOverlapCallbacks,
                              [&](auto& beginOverlapEvent)
                              { beginOverlapEvent(*b); });
        std::ranges::for_each(b->m_BeginOverlapCallbacks,
                              [&](auto& endOverlapEvent)
                              { endOverlapEvent(*a); });
    }

    ResolveCollision(a, b);
}

void dae::CollisionSystem::CleanupEndedOverlaps()
{
    for (auto* collider : m_DynamicColliders)
    {
        std::erase_if(
            collider->m_CurrentOverlaps,
            [&](ColliderComponent* other)
            {
                if (!collider->IsColliding(*other))
                {
                    std::ranges::for_each(collider->m_EndOverlapCallbacks,
                                          [&](auto& cb) { cb(*other); });
                    std::ranges::for_each(other->m_EndOverlapCallbacks,
                                          [&](auto& cb) { cb(*collider); });

                    other->RemoveOverlap(collider);
                    return true;
                }
                return false;
            });
    }
}