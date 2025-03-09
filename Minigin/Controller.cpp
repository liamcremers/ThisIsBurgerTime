#include "Controller.h"
//#include <Xinput.h>

#include <unordered_map>

//#include <minwinbase.h>

namespace dae
{
    class Controller::ControllerImpl
    {
    public:
        void ProcessInput();
        bool IsDownThisFrame(unsigned int button) const;
        bool IsUpThisFrame(unsigned int button) const;
        bool IsPressed(unsigned int button) const;

    private:
        //DWORD m_controllerIndex{ 0 };
        //XINPUT_STATE m_PreviousState{};
        //XINPUT_STATE m_CurrentState{};
        int m_ButtonsPressedThisFrame{};
        int m_ButtonsReleasedThisFrame{};

        //std::unordered_map<int, std::pair<Command&, int>> m_CommandsMap;
    };

    void Controller::ControllerImpl::ProcessInput()
    {
        //CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
        //ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
        //XInputGetState(m_controllerIndex, &m_CurrentState);

        //auto buttonChanges =
            //m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;

        //m_ButtonsPressedThisFrame =
            //buttonChanges & m_CurrentState.Gamepad.wButtons;

        //m_ButtonsReleasedThisFrame =
            //buttonChanges & (~m_CurrentState.Gamepad.wButtons);
    }

    bool Controller::ControllerImpl::IsDownThisFrame(unsigned int button) const
    {
        return m_ButtonsPressedThisFrame & button;
    }

    bool Controller::ControllerImpl::IsUpThisFrame(unsigned int button) const
    {
        return m_ButtonsReleasedThisFrame & button;
    }

    bool Controller::ControllerImpl::IsPressed(unsigned int ) const
    {
        //return m_CurrentState.Gamepad.wButtons & button;
        return false;
    }

    ////////////////////////
    // Controller
    ////////////////////////
    Controller::Controller() :
        m_pImpl{ new ControllerImpl() }
    {}

    Controller::~Controller() { delete m_pImpl; }

    bool Controller::ProcessInput()
    {
        m_pImpl->ProcessInput();
        return true;
    }

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
}
