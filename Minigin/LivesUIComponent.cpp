#include "LivesUIComponent.h"
#include "LivesComponent.h"
#include "GameObject.h"

dae::LivesUIComponent::LivesUIComponent(GameObject& parent, Font& font) :
    BaseComponent{ parent },
    m_pFont{ font },
    m_pTextComponent{ parent.AddComponent<TextComponent>("Lives: ", font) },
    m_pLivesComponent{ parent.TryGetComponent<LivesComponent>().has_value() ?
                           parent.TryGetComponent<LivesComponent>().value() :
                           nullptr }
{
    assert(m_pLivesComponent && "LivesUIComponent depends on LivesComponent");
    m_pLivesComponent->AddObserver(this);
}

void dae::LivesUIComponent::OnNotify(const std::string& eventId)
{
    if (eventId == "LifeLost")
        UpdateUI();
    if (eventId == "GameOver")
        UIGameOver();
}

void dae::LivesUIComponent::UpdateUI()
{
    m_pTextComponent->SetText("Lives: " +
                              std::to_string(m_pLivesComponent->GetLives()));
}

void dae::LivesUIComponent::UIGameOver()
{
    m_pTextComponent->SetText("GAME OVER!");
}
