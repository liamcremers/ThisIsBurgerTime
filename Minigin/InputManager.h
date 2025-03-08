#pragma once
#include "Singleton.h"
#include "SceneManager.h"

#include <unordered_map>
#include "Controller.h"

namespace dae
{
    class Controller;
    class Command;
    class KeyState;
    class ControllerButton;
    class KeyboardButton;

    class InputManager final : public Singleton<InputManager>
    {
    public:
        bool ProcessInput();

        bool IsDownThisFram(SDL_Event& event, SDL_Keysym key) const;
        bool IsUpThisFram(SDL_Event& event, SDL_Keysym key) const;
        bool IsPressed(SDL_Keysym key) const;

        void AddController();
        void AddControllerCommand(Command& pCommand,
                                  int button,
                                  KeyState keyState,
                                  uint8_t controllerIdx);

    private:
        std::vector<Controller> m_ControllerVec;
    };
}
