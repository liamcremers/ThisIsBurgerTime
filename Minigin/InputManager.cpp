#include <SDL.h>
#include "InputManager.h"
#include <backends/imgui_impl_sdl2.h>

auto dae::InputManager::ProcessInput() -> bool
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            return false;

        if (e.type == SDL_KEYDOWN)
            IsPressed(e.key.keysym.sym);
        if (e.type == SDL_MOUSEBUTTONDOWN)

            ImGui_ImplSDL2_ProcessEvent(&e);
    }
    return true;
}

bool dae::InputManager::IsPressed(SDL_Keysym key) const { return false; }

void dae::InputManager::AddController()
{
    if (m_m_ControllerVec.size() < 2)
        m_ControllerVec.emplace_back(Controller());
}
