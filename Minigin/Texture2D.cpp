#include <SDL.h>
#include <SDL_image.h>
#include "Texture2D.h"
#include "Renderer.h"
#include <stdexcept>

dae::Texture2D::~Texture2D() { SDL_DestroyTexture(m_texture); }

auto dae::Texture2D::GetSize() const -> glm::ivec2
{
    SDL_Rect dst{};
    SDL_QueryTexture(GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
    return { dst.w, dst.h };
}

auto dae::Texture2D::IsScaled() const -> bool { return m_isScaled; }

void dae::Texture2D::Scale(float scale)
{
    if (scale == 1.0f)
        return; // No scaling needed

    m_isScaled = true;
    auto size = GetSize();
    size[0] = size[0] * static_cast<int>(scale);
    size[1] = size[1] * static_cast<int>(scale);

    SDL_Texture* newTexture =
        SDL_CreateTexture(Renderer::GetInstance().GetSDLRenderer(),
                          SDL_PIXELFORMAT_RGBA8888,
                          SDL_TEXTUREACCESS_TARGET,
                          size[0],
                          size[1]);

    if (!newTexture)
        throw std::runtime_error("Failed to create scaled texture: " +
                                 std::string(SDL_GetError()));

    SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(Renderer::GetInstance().GetSDLRenderer(), newTexture);
    SDL_Rect dst{ 0, 0, size[0], size[1] };
    SDL_RenderCopy(
        Renderer::GetInstance().GetSDLRenderer(), m_texture, nullptr, &dst);
    SDL_SetRenderTarget(Renderer::GetInstance().GetSDLRenderer(), nullptr);

    SDL_DestroyTexture(m_texture);
    m_texture = newTexture;
}

auto dae::Texture2D::GetSDLTexture() const -> SDL_Texture* { return m_texture; }

dae::Texture2D::Texture2D(const std::string& fullPath) :
    m_texture{ IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(),
                               fullPath.c_str()) }
{
    if (m_texture == nullptr)
        throw std::runtime_error(std::string("Failed to load texture: ") +
                                 SDL_GetError());
}

dae::Texture2D::Texture2D(SDL_Texture* texture) :
    m_texture{ texture }
{
    assert(m_texture != nullptr);
}
