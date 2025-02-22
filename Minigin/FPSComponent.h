#pragma once
#include "BaseComponent.h"
#include "ComponentsHeader.h"
#include "EngineTime.h"

namespace dae
{
	class FPSComponent final : public BaseComponent
	{
	public:
		FPSComponent(GameObject& parent);
		void Update() override;
	private:
		void Render() const override {};
		void FixedUpdate() override {};
		void LateUpdate() override {};
	};
}
