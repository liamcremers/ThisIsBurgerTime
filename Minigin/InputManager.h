#pragma once
#include "Singleton.h"

#include <vector>
#include <unordered_map>

typedef int SDL_Keycode;

namespace dae
{
    class Controller;
    class Command;

    class InputManager final : public Singleton<InputManager>
    {
    public:
        bool ProcessInput();

        void AddController(Controller* controller);
        void AddKeyboardCommand(SDL_Keycode keyboardButton, Command* command);

    private:
        std::vector<Controller*> m_ControllerVec;
        std::unordered_map<SDL_Keycode, Command*> m_KeyCommandMap;
    };
}