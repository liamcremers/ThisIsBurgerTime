#include "Controller.h"
#include <Xinput.h>

#include <unordered_map>
#include <minwinbase.h>

class dae::Controller::ControllerImpl
{
public:
    void ProcessInput();
    bool IsDownThisFrame(unsigned int button) const;
    bool IsUpThisFrame(unsigned int button) const;
    bool IsPressed(unsigned int button) const;

private:
    DWORD m_controllerIndex{ 0 };
    XINPUT_STATE m_PreviousState{};
    XINPUT_STATE m_CurrentState{};
    int m_ButtonsPressedThisFrame{};
    int m_ButtonsReleasedThisFrame{};

    std::unordered_map<int, std::pair<Command&, int>> m_CommandsMap;
};

void dae::Controller::ControllerImpl::ProcessInput()
{
    CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
    ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
    XInputGetState(m_controllerIndex, &m_CurrentState);

    auto buttonChanges =
        m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;

    m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;

    m_ButtonsReleasedThisFrame =
        buttonChanges & (~m_CurrentState.Gamepad.wButtons);
}

bool dae::Controller::ControllerImpl::IsDownThisFrame(unsigned int button) const
{
    return m_ButtonsPressedThisFrame & button;
}

bool dae::Controller::ControllerImpl::IsUpThisFrame(unsigned int button) const
{
    return m_ButtonsReleasedThisFrame & button;
}

bool dae::Controller::ControllerImpl::IsPressed(unsigned int button) const
{
    return m_CurrentState.Gamepad.wButtons & button;
}

////////////////////////
// Controller
////////////////////////
dae::Controller::Controller() :
    m_pImpl{ std::make_unique<ControllerImpl>() }
{}

bool dae::Controller::ProcessInput()
{
    m_pImpl->ProcessInput();
    return true;
}

bool dae::Controller::IsDownThisFrame(unsigned int button) const
{
    return m_pImpl->IsDownThisFrame(button);
}

bool dae::Controller::IsUpThisFrame(unsigned int button) const
{
    return m_pImpl->IsUpThisFrame(button);
}

bool dae::Controller::IsPressed(unsigned int button) const
{
    return m_pImpl->IsPressed(button);
}
