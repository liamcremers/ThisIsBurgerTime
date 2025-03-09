#include "InputManager.h"
#include "Command.h"
#include "Controller.h"

#include <SDL.h>
#include <backends/imgui_impl_sdl2.h>
#include <ranges>
#include <algorithm>

auto dae::InputManager::ProcessInput() -> bool
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        ImGui_ImplSDL2_ProcessEvent(&e);

        if (e.type == SDL_QUIT)
        {
            return false;
        }

        std::ranges::for_each(m_ControllerVec, &Controller::ProcessInput);
    }
    return true;
}

void dae::InputManager::AddController(Controller* controller)
{
    if (m_ControllerVec.size() < 2)
        m_ControllerVec.emplace_back(controller);
}
