#pragma once  
#include "BaseComponent.h"  
#include "TransformComponent.h"  
#include "EngineTime.h"  

namespace dae
{
	class RotatorComponent final : public BaseComponent
	{
	public:
		RotatorComponent(GameObject& owner, int radius, bool clockwise = true);
		void Update() override;
	private:
		int m_AngleSpeed{ 60 };
		int m_Radius{};
		bool m_Clockwise{};
		float m_Angle{};
	};
}
