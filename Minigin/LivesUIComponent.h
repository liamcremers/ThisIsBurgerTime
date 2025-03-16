#pragma once
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
    class Font;
    class LivesComponent;
    class TextComponent;

    class LivesUIComponent final : public BaseComponent, public Observer
    {
    public:
        LivesUIComponent(GameObject& parent, Font& font);
        ~LivesUIComponent() override;

    protected:
        void OnNotify(const std::string& eventId) override;

    private:
        void UpdateUI();
        void UIGameOver();

        Font& m_pFont;
        LivesComponent* m_pLivesComponent{};
        TextComponent* m_pTextComponent{};
    };
}
