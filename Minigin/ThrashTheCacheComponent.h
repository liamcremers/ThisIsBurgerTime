#pragma once  
#include "BaseComponent.h"  

namespace dae
{
	class ThrashTheCacheComponent final : public BaseComponent
	{
	public:
		ThrashTheCacheComponent(GameObject& owner);
		void RenderUI() const override;
	private:
	};
}
