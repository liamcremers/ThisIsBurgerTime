#pragma once
#include "BaseComponent.h"
#include "GameObject.h"
#include "ComponentsHeader.h"
#include "EngineTime.h"

namespace dae
{
	class FPSComponent final : public BaseComponent
	{
	public:
		FPSComponent(GameObject* parent);
		void Update() override;
		void Render() const override;
	};
}
