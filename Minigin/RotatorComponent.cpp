#include "RotatorComponent.h"
#include "GameObject.h"

dae::RotatorComponent::RotatorComponent(GameObject& owner,
                                        float radius,
                                        bool clockwise) :
    BaseComponent(owner),
    m_Radius{ radius },
    m_Clockwise{ clockwise }
{
    assert("Rotator needs a parent" && (GetOwner().GetParent() != nullptr));
}

void dae::RotatorComponent::Update()
{
    auto deltaTime = EngineTime::GetInstance().GetDeltaTime();
    m_Angle += static_cast<float>(m_Clockwise ? 1 : -1) *
               static_cast<float>(m_AngleSpeed) * deltaTime;
    m_Angle = fmod(m_Angle, m_AngleMax);

    glm::vec2 pos{};
    float angleRad = glm::radians(m_Angle);

    pos.x = m_Radius * cos(angleRad);

    pos.y = m_Radius * sin(angleRad);

    GetOwner().SetLocalPosition(pos);
}
