#include <stdexcept>
#include <SDL_ttf.h>
#include "Font.h"

auto dae::Font::GetFont() const -> _TTF_Font* { return m_font; }

dae::Font::Font(const std::string& fullPath, int size) :
    m_font{ TTF_OpenFont(fullPath.c_str(), size) }
{
    if (m_font == nullptr)
    {
        throw std::runtime_error(std::string("Failed to load font: ") +
                                 SDL_GetError());
    }
}

dae::Font::~Font() { TTF_CloseFont(m_font); }
