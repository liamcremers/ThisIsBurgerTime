#pragma once
#include "BaseComponent.h"

#include <memory>

namespace dae
{
    class Font;
    class TextComponent;

    class FPSComponent final : public BaseComponent
    {
    public:
        FPSComponent(GameObject& parent, Font& font);
        void Update() override;

    private:
        Font& m_pFont;
        TextComponent* m_pTextComponent;
    };
}
