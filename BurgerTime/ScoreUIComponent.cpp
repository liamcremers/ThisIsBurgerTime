#include "ScoreUIComponent.h"
#include "ScoreComponent.h"
#include <GameObject.h>
#include <TextComponent.h>
#include <Font.h>
#include <cassert>
#include <string>

ScoreUIComponent::ScoreUIComponent(dae::GameObject& parent,
                                   dae::Font& font,
                                   ScoreComponent* pScoreComp) :
    BaseComponent{ parent },
    m_pScoreComponent{ pScoreComp },
    m_pTextComponent{ parent.AddComponent<dae::TextComponent>(
        "Score: " + std::to_string(m_pScoreComponent->GetScore()),
        font) }
{
    assert(m_pScoreComponent && "ScoreUIComponent depends on ScoreComponent");
    m_pScoreComponent->GetScoreSubject().AddObserver(this);
}

ScoreUIComponent::~ScoreUIComponent()
{
    if (m_pScoreComponent)
        m_pScoreComponent->GetScoreSubject().RemoveObserver(this);
}

void ScoreUIComponent::OnNotify(const std::string& eventId,
                                [[maybe_unused]] const std::any& args)
{
    if (eventId == "ScoreUpdated")
        UpdateUI();
}

void ScoreUIComponent::OnDestroy() { m_pScoreComponent = nullptr; }

void ScoreUIComponent::UpdateUI()
{
    m_pTextComponent->SetText("Score: " +
                              std::to_string(m_pScoreComponent->GetScore()));
}
