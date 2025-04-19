#pragma once
#include "BaseComponent.h"
#include "glm.hpp"

namespace dae
{
    class ColliderComponent;

    class PhysicsComponent : public BaseComponent
    {
    public:
        PhysicsComponent(GameObject& parent, bool useGravity = true);
        void FixedUpdate() override;

        void SetVelocity(const glm::vec2& velocity);
        void SetGravity(float gravity);
        void SetMaxFallSpeed(float speed);
        void SetUseGravity(bool useGravity);

        [[nodiscard]] glm::vec2 GetVelocity() const;
        [[nodiscard]] float GetGravity() const;
        [[nodiscard]] float GetMaxFallSpeed() const;
        [[nodiscard]] bool IsUsingGravity() const;

    private:
        ColliderComponent* m_pCollider{ nullptr };

        glm::vec2 m_Velocity{ 0, 0 };
        float m_Gravity{ 9.81f };
        float m_MaxFallSpeed{ 1000.f };
        bool m_UseGravity{ true };
    };
}
