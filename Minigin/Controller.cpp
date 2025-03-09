#include "Controller.h"
#include "Command.h"

#include <Xinput.h>
#include <minwinbase.h>
#include <unordered_map>
#include <string>

namespace dae
{
    class Controller::ControllerImpl
    {
    public:
        explicit ControllerImpl(DWORD idx) :
            m_ControllerIndex{ idx }
        {}

        void ProcessInput();
        [[nodiscard]] auto IsDownThisFrame(unsigned int button) const -> bool;
        [[nodiscard]] auto IsUpThisFrame(unsigned int button) const -> bool;
        [[nodiscard]] auto IsPressed(unsigned int button) const -> bool;
        [[nodiscard]] auto IsReleased(unsigned int button) const -> bool;

        void AddCommand(Command& pCommand, unsigned int button);

    private:
        DWORD m_ControllerIndex{};
        XINPUT_STATE m_PreviousState{};
        XINPUT_STATE m_CurrentState{};
        int m_ButtonsPressedThisFrame{};
        int m_ButtonsReleasedThisFrame{};

        std::unordered_map<unsigned int, Command*> m_CommandsMap;
    };

    void Controller::ControllerImpl::ProcessInput()
    {
        CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
        ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
        XInputGetState(m_ControllerIndex, &m_CurrentState);

        auto buttonChanges =
            m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;

        m_ButtonsPressedThisFrame =
            buttonChanges & m_CurrentState.Gamepad.wButtons;
        m_ButtonsReleasedThisFrame =
            buttonChanges & (~m_CurrentState.Gamepad.wButtons);

        for (auto& [key, command] : m_CommandsMap)
        {
            if (IsDownThisFrame(key))
            {
                //command->Execute();
                //break;
            }
            if (IsUpThisFrame(key))
            {
                //command->Execute();
                //break;
            }
            if (IsPressed(key))
            {
                command->Execute();
                break;
            }
            if (IsReleased(key))
            {
                //command->Execute();
                //break;
            }
        }
    }

    auto Controller::ControllerImpl::IsDownThisFrame(unsigned int button) const
        -> bool
    {
        return m_ButtonsPressedThisFrame & button;
    }

    auto Controller::ControllerImpl::IsUpThisFrame(unsigned int button) const
        -> bool
    {
        return m_ButtonsReleasedThisFrame & button;
    }

    auto Controller::ControllerImpl::IsPressed(unsigned int button) const
        -> bool
    {
        return m_CurrentState.Gamepad.wButtons & button;
    }

    auto Controller::ControllerImpl::IsReleased(unsigned int button) const
        -> bool
    {
        return !(m_CurrentState.Gamepad.wButtons & button);
    }

    void Controller::ControllerImpl::AddCommand(Command& pCommand,
                                                unsigned int button)
    {
        m_CommandsMap[button] = &pCommand;
    }

    ////////////////////////
    // Controller
    ////////////////////////

    Controller::Controller(unsigned long idx) :
        m_pImpl{ new ControllerImpl(idx) }
    {}

    Controller::~Controller() { delete m_pImpl; }

    void Controller::ProcessInput() { m_pImpl->ProcessInput(); }

    void Controller::AddCommand(Command& pCommand, unsigned int button)
    {
        m_pImpl->AddCommand(pCommand, button);
    }

bool dae::Controller::IsPressed(unsigned int button) const
{
    return m_pImpl->IsPressed(button);
}
