#pragma once
#include <BaseComponent.h>

namespace dae
{
    class Font;
    class TextComponent;
}

class FPSComponent final : public dae::BaseComponent
{
public:
    explicit FPSComponent(dae::GameObject& parent, dae::Font& font);
    void Update() override;

private:
    dae::TextComponent* m_pTextComponent{};
};
