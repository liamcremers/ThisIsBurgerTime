#include "LivesUIComponent.h"
#include "LivesComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "Font.h"
#include <cassert>
#include <string>

LivesUIComponent::LivesUIComponent(dae::GameObject& parent,
                                   dae::Font& font,
                                   LivesComponent* pLivesComponent) :
    BaseComponent{ parent },
    m_pLivesComponent{ pLivesComponent },
    m_pTextComponent{ parent.AddComponent<dae::TextComponent>(
        "Lives: " + std::to_string(m_pLivesComponent->GetLives()),
        font) }
{
    assert(m_pLivesComponent && "LivesUIComponent depends on LivesComponent");
    m_pLivesComponent->GetLivesSubject().AddObserver(this);
}

LivesUIComponent::~LivesUIComponent()
{
    if (m_pLivesComponent)
        m_pLivesComponent->GetLivesSubject().RemoveObserver(this);
}

void LivesUIComponent::OnNotify(const std::string& eventId,
                                [[maybe_unused]] const std::any& args)
{
    if (eventId == "LifeLost")
        UpdateUI();
    else if (eventId == "GameOver")
        UIGameOver();
}

void LivesUIComponent::OnDestroy() { m_pLivesComponent = nullptr; }

void LivesUIComponent::UpdateUI()
{
    m_pTextComponent->SetText("Lives: " +
                              std::to_string(m_pLivesComponent->GetLives()));
}

void LivesUIComponent::UIGameOver() { m_pTextComponent->SetText("GAME OVER!"); }
