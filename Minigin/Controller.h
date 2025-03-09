#pragma once
#include "Command.h"

#include <memory>

namespace dae
{
    class Command;

    class Controller
    {
    public:
        Controller(unsigned long idx);
        ~Controller();
        void ProcessInput();
        void AddCommand(Command& pCommand, unsigned int button);

    private:
        class ControllerImpl;
        ControllerImpl* m_pImpl;
    };
}
