#pragma once

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
        void RemoveCommand(Command& pCommand, unsigned int button);

    private:
        class ControllerImpl;
        ControllerImpl* m_pImpl;
    };
}
