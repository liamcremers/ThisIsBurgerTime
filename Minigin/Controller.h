#pragma once
#include "Command.h"

#include <memory>

namespace dae
{
    class Controller
    {
    public:
        Controller();

        bool ProcessInput();
        bool IsDownThisFrame(unsigned int button) const;
        bool IsUpThisFrame(unsigned int button) const;
        bool IsPressed(unsigned int button) const;

    private:
        class ControllerImpl;
        std::unique_ptr<ControllerImpl> m_pImpl;
    };
}
