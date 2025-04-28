#ifdef DEBUG_RENDER
#pragma once
#include "Singleton.h"
#include "Renderer.h"
#include "Font.h"
#include "ResourceManager.h"
#include <glm.hpp>
#include <vector>
#include <SDL.h>
#include <string>

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

        struct DebugText
        {
            std::string text;
            std::chrono::steady_clock::time_point time;
        };

    public:
        void RenderRect(const glm::vec2& position,
                        const glm::vec2& size,
                        const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

        void RenderText(const std::string& text);

        void Flush();

    private:
        friend class Singleton<DebugRenderer>;
        DebugRenderer() = default;

        static constexpr int SMALL_FONT_SIZE = 14;
        static constexpr int DEBUG_TEXT_DURATION = 2;
        static constexpr int DEBUG_TEXT_PADDING = 15;
        static constexpr int DEBUG_TEXT_PADDING_X = 55;
        static constexpr auto DEBUG_TEXT_COLOR = SDL_Color{ 0, 170, 255, 255 };

        std::vector<DebugRect> m_DebugRects;
        std::vector<DebugText> m_DebugTexts;
        dae::Font& m_Font{ dae::ResourceManager::GetInstance().LoadFont(
            "Lingua.otf",
            SMALL_FONT_SIZE) };
        SDL_Renderer* m_Renderer = nullptr;
    };
}
#endif // DEBUG_RENDER