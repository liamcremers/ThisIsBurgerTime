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
    m_pTextComponent{ parent.AddComponent<TextComponent>("Score: ", font) },
    m_pScoreComponent{ pScoreComp }
{
    assert(m_pScoreComponent && "ScoreUIComponent depends on ScoreComponent");
    m_pScoreComponent->AddObserver(this);
}

dae::ScoreUIComponent::~ScoreUIComponent()
{
    if (m_pScoreComponent)
        m_pScoreComponent->RemoveObserver(this);
}

void dae::ScoreUIComponent::OnNotify(const std::string& eventId)
{
    if (eventId == "ScoreAdded")
        UpdateUI();
    else if (eventId == "SubjectDestroyed")
        m_pScoreComponent = nullptr;
}

void dae::ScoreUIComponent::UpdateUI()
{
    m_pTextComponent->SetText("Score: " +
                              std::to_string(m_pScoreComponent->GetScore()));
}
