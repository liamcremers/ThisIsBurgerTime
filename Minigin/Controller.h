#pragma once

//#include "Command.h"

namespace dae
{
    class Controller
    {
    public:
        Controller();
        ~Controller();
        bool ProcessInput();
        bool IsDownThisFrame(unsigned int button) const;
        bool IsUpThisFrame(unsigned int button) const;
        bool IsPressed(unsigned int button) const;

    private:
        class ControllerImpl;
        ControllerImpl* m_pImpl;
    };
}
