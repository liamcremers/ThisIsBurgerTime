#pragma once
#include "Singleton.h"

#include <vector>

namespace dae
{
    class Controller;
    class Command;
    class ControllerButton;
    class KeyboardButton;

    class InputManager final : public Singleton<InputManager>
    {
    public:
        bool ProcessInput();

        void AddController(Controller* controller);

    private:
        std::vector<Controller*> m_ControllerVec;
    };
}
