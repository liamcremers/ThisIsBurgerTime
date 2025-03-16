#pragma once
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
    class Font;
    class ScoreComponent;
    class TextComponent;
    class ScoreComponent;

    class ScoreUIComponent final : public BaseComponent, public Observer
    {
    public:
        ScoreUIComponent(GameObject& parent,
                         Font& font,
                         ScoreComponent* pScoreComp);
        ~ScoreUIComponent() override;

    protected:
        void OnNotify(const std::string& eventId) override;

    private:
        void UpdateUI();

        Font& m_pFont;
        ScoreComponent* m_pScoreComponent{};
        TextComponent* m_pTextComponent{};
    };
}
