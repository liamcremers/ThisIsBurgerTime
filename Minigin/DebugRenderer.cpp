#ifdef DEBUG_RENDER
#include "DebugRenderer.h"

#include <SDL_ttf.h>
#include <algorithm>
#include <chrono>
#include <ranges>

void dae::DebugRenderer::RenderRect(const glm::vec2& position,
                                    const glm::vec2& size,
                                    const glm::vec4& color)
{
    m_DebugRects.emplace_back(
        DebugRect{ .position = position, .size = size, .color = color });
}

void dae::DebugRenderer::RenderText(const std::string& text)
{
    m_DebugTexts.emplace_back(text, std::chrono::steady_clock::now());
}

void dae::DebugRenderer::Flush()
{
    if (!m_Renderer)
        m_Renderer = dae::Renderer::GetInstance().GetSDLRenderer();

    Uint8 r = 0;
    Uint8 g = 0;
    Uint8 b = 0;
    Uint8 a = 0;
    SDL_GetRenderDrawColor(m_Renderer, &r, &g, &b, &a);

    static constexpr int MAX_VALUE = 255;
    auto now = std::chrono::steady_clock::now();

    // Render Rectangles
    for (const auto& rect : m_DebugRects)
    {
        SDL_FRect sdlRect = {
            rect.position[0], rect.position[1], rect.size[0], rect.size[1]
        };
        SDL_SetRenderDrawColor(m_Renderer,
                               static_cast<Uint8>(rect.color[0] * MAX_VALUE),
                               static_cast<Uint8>(rect.color[1] * MAX_VALUE),
                               static_cast<Uint8>(rect.color[2] * MAX_VALUE),
                               static_cast<Uint8>(rect.color[3] * MAX_VALUE));
        SDL_RenderDrawRectF(m_Renderer, &sdlRect);
    }

    // Render Texts
    for (size_t i = 0; i < m_DebugTexts.size(); ++i)
    {
        const auto& text = m_DebugTexts[i].text;
        SDL_Surface* surface = TTF_RenderText_Solid(
            m_Font.GetFont(), text.c_str(), DEBUG_TEXT_COLOR);
        SDL_Texture* texture =
            SDL_CreateTextureFromSurface(m_Renderer, surface);
        SDL_FreeSurface(surface);

        SDL_Rect dstRect = { DEBUG_TEXT_PADDING_X,
                             static_cast<int>(DEBUG_TEXT_PADDING +
                                              i * DEBUG_TEXT_PADDING),
                             0,
                             0 };
        SDL_QueryTexture(texture, nullptr, nullptr, &dstRect.w, &dstRect.h);
        SDL_RenderCopy(m_Renderer, texture, nullptr, &dstRect);
        SDL_DestroyTexture(texture);
    }

    // Remove expired texts
    auto [new_end, old_end] = std::ranges::remove_if(
        m_DebugTexts,
        [now](const DebugText& text)
        {
            return std::chrono::duration_cast<std::chrono::seconds>(now -
                                                                    text.time)
                       .count() > DEBUG_TEXT_DURATION;
        });
    m_DebugTexts.erase(new_end, m_DebugTexts.end());

    SDL_SetRenderDrawColor(m_Renderer, r, g, b, a);
    m_DebugRects.clear();
}
#endif // DEBUG_RENDER
