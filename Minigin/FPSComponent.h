#pragma once
#include <memory>
#include "Font.h"
#include "BaseComponent.h"
#include "ComponentsHeader.h"
#include "EngineTime.h"
#include "TextComponent.h" 


namespace dae
{
	class FPSComponent final : public BaseComponent
	{
	public:
		FPSComponent(GameObject& parent, Font& font);
		void Update() override;
	private:
		Font& m_pFont;
		TextComponent* m_pTextComponent{ nullptr };
	};
}

