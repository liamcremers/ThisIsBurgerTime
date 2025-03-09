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
        void AddControllerCommand(Command& pCommand,
                                  int button,
                                  uint8_t controllerIdx);

    private:
        std::vector<Controller*> m_ControllerVec;
    };
}
