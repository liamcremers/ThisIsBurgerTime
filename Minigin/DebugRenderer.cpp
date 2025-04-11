#include "DebugRenderer.h"

//TODO: optimize this using a better container
void DebugRenderer::RenderRect(const glm::vec2& position,
                               const glm::vec2& size,
                               const glm::vec4& color)
{
    m_DebugRects.emplace_back(position, size, color);
}

void DebugRenderer::Flush()
{
    if (!m_Renderer)
        m_Renderer = dae::Renderer::GetInstance().GetSDLRenderer();

    Uint8 r = 0;
    Uint8 g = 0;
    Uint8 b = 0;
    Uint8 a = 0;
    SDL_GetRenderDrawColor(m_Renderer, &r, &g, &b, &a);

    for (const auto& rect : m_DebugRects)
    {
        SDL_FRect sdlRect = {
            rect.position[0], rect.position[1], rect.size[0], rect.size[1]
        };

        SDL_SetRenderDrawColor(m_Renderer,
                               static_cast<Uint8>(rect.color[0] * 255),
                               static_cast<Uint8>(rect.color[1] * 255),
                               static_cast<Uint8>(rect.color[2] * 255),
                               static_cast<Uint8>(rect.color[3] * 255));
        SDL_RenderDrawRectF(m_Renderer, &sdlRect);
    }
    SDL_SetRenderDrawColor(m_Renderer, r, g, b, a);

    m_DebugRects.clear();
}
