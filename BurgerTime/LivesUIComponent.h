#pragma once
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
    class Font;
    class TextComponent;
}
class LivesComponent;

class LivesUIComponent final : public dae::BaseComponent, public dae::Observer
{
public:
    explicit LivesUIComponent(dae::GameObject& parent,
                              dae::Font& font,
                              LivesComponent* pLivesComponent);
    ~LivesUIComponent() override;

protected:
    void OnNotify(const std::string& eventId,
                  const std::any& args = std::any{}) override;
    void OnDestroy() override;

private:
    void UpdateUI();
    void UIGameOver();

    LivesComponent* m_pLivesComponent{};
    dae::TextComponent* m_pTextComponent{};
};
