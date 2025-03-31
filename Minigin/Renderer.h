#pragma once
#include <SDL.h>
#include "Singleton.h"

namespace dae
{
    class Texture2D;

    /**
	 * Simple RAII wrapper for the SDL renderer
	 */
    class Renderer final : public Singleton<Renderer>
    {
    public:
        void Init(SDL_Window* window);
        void Render() const;
        void Destroy();

        void RenderTexture(const Texture2D& texture,
                           const float x,
                           const float y) const;
        void RenderTexture(const Texture2D& texture,
                           const float x,
                           const float y,
                           const float width,
                           const float height) const;

        SDL_Renderer* GetSDLRenderer() const;
        const SDL_Color& GetBackgroundColor() const;
        void SetBackgroundColor(const SDL_Color& color);

    private:
        friend class Singleton<Renderer>;
        Renderer() = default;

        SDL_Renderer* m_renderer{};
        SDL_Window* m_window{};
        SDL_Color m_clearColor{};
    };
}
