#pragma once

namespace dae
{
    class Command;

    class Controller
    {
    public:
        Controller(unsigned long idx);
        ~Controller();
        bool ProcessInput();
        bool IsDownThisFrame(unsigned int button) const;
        bool IsUpThisFrame(unsigned int button) const;
        bool IsPressed(unsigned int button) const;
        bool IsReleased(unsigned int button) const;

        void AddCommand(Command& pCommand, unsigned int button);
    private:
        class ControllerImpl;
        ControllerImpl* m_pImpl;
    };
}
