#include "TextComponent.h"
#include <stdexcept>
#include <SDL_ttf.h>

#include "GameObject.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

dae::TextComponent::TextComponent(GameObject& parent,
                                  const std::string& text,
                                  Font& font) :
    BaseComponent(parent),
    m_needsUpdate(true),
    m_text(text),
    m_font(font),
    m_textTexture(nullptr),
    m_pTransform(parent.GetComponent<TransformComponent>())
{}

void dae::TextComponent::Update()
{
    if (m_needsUpdate)
    {
        const SDL_Color color = { 255, 255, 255, 255 };
        const auto surf =
            TTF_RenderText_Blended(m_font.GetFont(), m_text.c_str(), color);
        if (surf == nullptr)
        {
            throw std::runtime_error(std::string("Render text failed: ") +
                                     SDL_GetError());
        }
        auto texture = SDL_CreateTextureFromSurface(
            Renderer::GetInstance().GetSDLRenderer(), surf);
        if (texture == nullptr)
        {
            throw std::runtime_error(
                std::string("Create text texture from surface failed: ") +
                SDL_GetError());
        }
        SDL_FreeSurface(surf);
        m_textTexture = std::make_unique<Texture2D>(texture);
        m_needsUpdate = false;
    }
}

void dae::TextComponent::Render() const
{
    if (m_textTexture != nullptr)
    {
        const auto& pos = GetOwner().GetWorldPosition();
        Renderer::GetInstance().RenderTexture(*m_textTexture, pos.x, pos.y);
    }
}

// This implementation uses the "dirty flag" pattern
void dae::TextComponent::SetText(const std::string& text)
{
    m_text = text;
    m_needsUpdate = true;
}

void dae::TextComponent::SetPosition(const float x, const float y)
{
    m_pTransform->SetWorldPosition({ x, y });
}
