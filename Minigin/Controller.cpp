#include "Controller.h"
#include "Command.h"
#include <Xinput.h>

#include <Windows.h>
#include <unordered_map>
#include <minwinbase.h>
#include <string>

namespace dae
{
    class Controller::ControllerImpl
    {
    public:
        explicit ControllerImpl(DWORD idx) :
            m_ControllerIndex{ idx }
        {}

        bool ProcessInput();
        bool IsDownThisFrame(unsigned int button) const;
        bool IsUpThisFrame(unsigned int button) const;
        bool IsPressed(unsigned int button) const;
        bool IsReleased(unsigned int button) const;

        void AddCommand(Command& pCommand, unsigned int button);

    private:
        DWORD m_ControllerIndex{};
        XINPUT_STATE m_PreviousState{};
        XINPUT_STATE m_CurrentState{};
        int m_ButtonsPressedThisFrame{};
        int m_ButtonsReleasedThisFrame{};

        std::unordered_map<unsigned int, Command*> m_CommandsMap;
    };

    bool Controller::ControllerImpl::ProcessInput()
    {
        CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
        ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
        XInputGetState(m_ControllerIndex, &m_CurrentState);

        auto buttonChanges =
            m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;

        m_ButtonsPressedThisFrame =
            buttonChanges & m_CurrentState.Gamepad.wButtons;
        OutputDebugString(
            (std::to_string(m_ButtonsPressedThisFrame) + '\n').c_str());

        m_ButtonsReleasedThisFrame =
            buttonChanges & (~m_CurrentState.Gamepad.wButtons);
        OutputDebugString(std::to_string(m_ButtonsReleasedThisFrame).c_str() +
                          '\n');

        for (auto& [key, command] : m_CommandsMap)
        {
            if (IsDownThisFrame(key))
            {
                command->Execute();
                OutputDebugString("down");
                break;
            }
            if (IsUpThisFrame(key))
            {
                command->Execute();
                OutputDebugString("up");
                break;
            }
            if (IsPressed(key))
            {
                command->Execute();
                OutputDebugString("pressed");
                break;
            }
            if (IsReleased(key))
            {
                //command->Execute();
                break;
            }
        }
        return true;
    }

    bool Controller::ControllerImpl::IsDownThisFrame(unsigned int button) const
    {
        return m_ButtonsPressedThisFrame & button;
    }

    bool Controller::ControllerImpl::IsUpThisFrame(unsigned int button) const
    {
        return m_ButtonsReleasedThisFrame & button;
    }

    bool Controller::ControllerImpl::IsPressed(unsigned int button) const
    {
        return m_CurrentState.Gamepad.wButtons & button;
    }

    bool Controller::ControllerImpl::IsReleased(unsigned int button) const
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

    bool Controller::ProcessInput() { return m_pImpl->ProcessInput(); }

    bool Controller::IsDownThisFrame(unsigned int button) const
    {
        return m_pImpl->IsDownThisFrame(button);
    }

    bool Controller::IsUpThisFrame(unsigned int button) const
    {
        return m_pImpl->IsUpThisFrame(button);
    }

    bool Controller::IsPressed(unsigned int button) const
    {
        return m_pImpl->IsPressed(button);
    }

    bool Controller::IsReleased(unsigned int button) const
    {
        return m_pImpl->IsReleased(button);
    }

    void Controller::AddCommand(Command& pCommand, unsigned int button)
    {
        m_pImpl->AddCommand(pCommand, button);
    }
}
