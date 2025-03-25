#pragma once

namespace dae
{
    enum class ButtonState
    {
        DownThisFrame,
        UpThisFrame,
        Pressed,
        Released
    };
    class Command;

    class Controller
    {
    public:
        Controller(unsigned long idx);
        ~Controller();
        void ProcessInput();
        void AddCommand(Command& pCommand,
                        unsigned int button,
                        ButtonState btnState);
        void RemoveCommand(Command& pCommand,
                           unsigned int button,
                           ButtonState btnState);

    private:
        class ControllerImpl;
        ControllerImpl* m_pImpl;
    };
}
