#pragma once
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
    class Font;
    class TextComponent;
}
class ScoreComponent;

class ScoreUIComponent final : public dae::BaseComponent, public dae::Observer
{
public:
    explicit ScoreUIComponent(dae::GameObject& parent,
                              dae::Font& font,
                              ScoreComponent* pScoreComp);
    ~ScoreUIComponent() override;

protected:
    void OnNotify(const std::string& eventId,
                  [[maybe_unused]] const std::any& args = std::any{}) override;
    void OnDestroy() override;

private:
    void UpdateUI();

    ScoreComponent* m_pScoreComponent{};
    dae::TextComponent* m_pTextComponent{};
};
