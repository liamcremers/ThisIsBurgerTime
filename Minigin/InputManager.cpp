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
            return false;
    }

    for (auto& [key, command] : m_KeyCommandMap)
    {
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == key)
        {
            command->Execute();
        }
        else if (e.type == SDL_KEYUP && e.key.keysym.sym == key)
        {
            // TODO: Implement
        }
    }
    std::ranges::for_each(m_ControllerVec, &Controller::ProcessInput);
    return true;
}

void dae::InputManager::AddController(Controller* controller)
{
    if (m_ControllerVec.size() < 2)
        m_ControllerVec.emplace_back(controller);
}

void dae::InputManager::AddKeyboardCommand(SDL_Keycode keyboardButton,
                                           Command* command)
{
    m_KeyCommandMap[keyboardButton] = command;
}
