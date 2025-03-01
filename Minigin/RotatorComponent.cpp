#include "RotatorComponent.h"  
#include "GameObject.h"

dae::RotatorComponent::RotatorComponent(GameObject& owner, int radius, bool clockwise)
	:BaseComponent(owner),
	m_AngleSpeed{ 60 },
	m_Radius{ radius },
	m_Clockwise{ clockwise },
	m_Angle{}
{
	//assert((GetOwner().GetParent()->IsChild(&GetOwner())));
}

void dae::RotatorComponent::Update()
{
	auto deltaTime = EngineTime::GetInstance().GetDeltaTime();
	m_Angle += (m_Clockwise ? 1 : -1) * m_AngleSpeed * deltaTime;
	m_Angle = fmod(m_Angle, 360.0f);

	glm::vec2 pos{};
	float angleRad = glm::radians(m_Angle);

	pos.x = m_Radius * cos(angleRad);

	pos.y = m_Radius * sin(angleRad);

	GetOwner().SetLocalPosition(pos);
}
