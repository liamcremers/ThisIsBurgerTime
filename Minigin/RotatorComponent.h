#pragma once
#include "BaseComponent.h"
#include "TransformComponent.h"
#include "EngineTime.h"

namespace dae
{
    class RotatorComponent final : public BaseComponent
    {
    public:
        RotatorComponent(GameObject& owner,
                         float radius,
                         bool clockwise = true);
        void Update() override;

    private:
        static constexpr float m_AngleMax{ 360.f };
        static constexpr int m_AngleSpeed{ 60 };

        float m_Angle{};
        float m_Radius{};
        bool m_Clockwise{};
    };
}
