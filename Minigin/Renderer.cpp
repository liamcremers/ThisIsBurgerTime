#include <stdexcept>
#include <cstring>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "DebugRenderer.h"
#include "CollisionSystem.h"

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

static auto GetOpenGLDriverIndex() -> int
{
    auto openglIndex = -1;
    const auto driverCount = SDL_GetNumRenderDrivers();
    for (auto i = 0; i < driverCount; i++)
    {
        SDL_RendererInfo info;
        if (!SDL_GetRenderDriverInfo(i, &info))
            if (!strcmp(info.name, "opengl"))
                openglIndex = i;
    }
    return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
    m_window = window;
    m_renderer = SDL_CreateRenderer(
        window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
    if (m_renderer == nullptr)
    {
        throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") +
                                 SDL_GetError());
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
    ImGui_ImplOpenGL3_Init("#version 130");
}

auto dae::Renderer::Render() const -> void
{
    const auto& color = GetBackgroundColor();
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(m_renderer);

    SceneManager::GetInstance().Render();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
#ifdef DEBUG_RENDER
#ifdef DEBUG_COLLIDER
    CollisionSystem::GetInstance().RenderColliders();
#endif //DEBUG_COLLIDER
#endif // DEBUG_RENDER
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

#ifdef DEBUG_RENDER
    DebugRenderer::GetInstance().Flush();
#endif // DEBUG_RENDER
    SDL_RenderPresent(m_renderer);
}

auto dae::Renderer::Destroy() -> void
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    if (m_renderer != nullptr)
    {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
}

auto dae::Renderer::RenderTexture(const Texture2D& texture,
                                  const float x,
                                  const float y) const -> void
{
    SDL_Rect dst{};
    dst.x = static_cast<int>(x);
    dst.y = static_cast<int>(y);
    SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
    SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

auto dae::Renderer::RenderTexture(const Texture2D& texture,
                                  const float x,
                                  const float y,
                                  const float width,
                                  const float height) const -> void
{
    SDL_Rect dst{};
    dst.x = static_cast<int>(x);
    dst.y = static_cast<int>(y);
    dst.w = static_cast<int>(width);
    dst.h = static_cast<int>(height);
    SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

auto dae::Renderer::GetSDLRenderer() const -> SDL_Renderer*
{
    return m_renderer;
}

auto dae::Renderer::GetBackgroundColor() const -> const SDL_Color&
{
    return m_clearColor;
}

void dae::Renderer::SetBackgroundColor(const SDL_Color& color)
{
    m_clearColor = color;
}
