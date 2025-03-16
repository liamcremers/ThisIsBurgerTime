#include "LivesUIComponent.h"
#include "LivesComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "Font.h"
#include <cassert>
#include <string>

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

dae::LivesUIComponent::~LivesUIComponent()
{
    if (m_pLivesComponent)
        m_pLivesComponent->RemoveObserver(this);
}

void dae::LivesUIComponent::OnNotify(const std::string& eventId)
{
    if (eventId == "LifeLost")
        UpdateUI();
    else if (eventId == "GameOver")
        UIGameOver();
    else if (eventId == "SubjectDestroyed")
        m_pLivesComponent = nullptr;
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
