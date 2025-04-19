#ifdef DEBUG_RENDER
#pragma once
#include "Singleton.h"
#include "Renderer.h"
#include <glm.hpp>
#include <vector>
#include <SDL.h>

class LevelGrid;

namespace dae
{
    class DebugRenderer : public dae::Singleton<DebugRenderer>
    {
        struct DebugRect
        {
            glm::vec2 position;
            glm::vec2 size;
            glm::vec4 color;
        };

    public:
        void RenderRect(const glm::vec2& position,
                        const glm::vec2& size,
                        const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

        void Flush();

    private:
        friend class Singleton<DebugRenderer>;
        DebugRenderer() = default;

        std::vector<DebugRect> m_DebugRects;
        SDL_Renderer* m_Renderer = nullptr;
    };
}
#endif // DEBUG_RENDER