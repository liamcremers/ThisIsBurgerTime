#include "ScoreUIComponent.h"
#include "ScoreComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "Font.h"
#include <cassert>
#include <string>

dae::ScoreUIComponent::ScoreUIComponent(GameObject& parent,
                                        Font& font,
                                        ScoreComponent* pScoreComp) :
    BaseComponent{ parent },
    m_pFont{ font },
    m_pScoreComponent{ pScoreComp },
    m_pTextComponent{ parent.AddComponent<TextComponent>(
        "Score: " + std::to_string(m_pScoreComponent->GetScore()),
        font) }
{
    assert(m_pScoreComponent && "ScoreUIComponent depends on ScoreComponent");
    m_pScoreComponent->GetScoreSubject().AddObserver(this);
}

dae::ScoreUIComponent::~ScoreUIComponent()
{
    if (m_pScoreComponent)
        m_pScoreComponent->GetScoreSubject().RemoveObserver(this);
}

void dae::ScoreUIComponent::OnNotify(const std::string& eventId)
{
    if (eventId == "ScoreUpdated")
        UpdateUI();
}

void dae::ScoreUIComponent::OnDestroy() { m_pScoreComponent = nullptr; }

void dae::ScoreUIComponent::UpdateUI()
{
    m_pTextComponent->SetText("Score: " +
                              std::to_string(m_pScoreComponent->GetScore()));
}
