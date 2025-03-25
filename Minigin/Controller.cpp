#include "Controller.h"
#include "Command.h"

#include <Xinput.h>
#include <minwinbase.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>

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

        void AddCommand(Command& pCommand,
                        unsigned int button,
                        ButtonState btnState);
        void RemoveCommand(Command& pCommand,
                           unsigned int button,
                           ButtonState btnState);

    private:
        DWORD m_ControllerIndex{};
        XINPUT_STATE m_PreviousState{};
        XINPUT_STATE m_CurrentState{};
        int m_ButtonsPressedThisFrame{};
        int m_ButtonsReleasedThisFrame{};

        std::unordered_map<unsigned int,
                           std::vector<std::pair<Command*, ButtonState>>>
            m_CommandsMap;
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

        for (auto& [key, commandStatePair] : m_CommandsMap)
        {
            for (auto& [command, state] : commandStatePair)
            {
                if (state == ButtonState::DownThisFrame && IsDownThisFrame(key))
                {
                    command->Execute();
                    break;
                }
                if (state == ButtonState::UpThisFrame && IsUpThisFrame(key))
                {
                    command->Execute();
                    break;
                }
                if (state == ButtonState::Pressed && IsPressed(key))
                {
                    command->Execute();
                    break;
                }
                if (state == ButtonState::Released && IsReleased(key))
                {
                    command->Execute();
                    break;
                }
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
                                                unsigned int button,
                                                ButtonState btnState)
    {
        m_CommandsMap[button].emplace_back(&pCommand, btnState);
    }

    void Controller::ControllerImpl::RemoveCommand(Command& pCommand,
                                                   unsigned int button,
                                                   ButtonState btnState)
    {
        auto& commands = m_CommandsMap[button];

        std::erase_if(commands,
                      [&pCommand, btnState](const auto& pair)
                      {
                          auto& [oCommand, oBtnState] = pair;
                          return oCommand == &pCommand && oBtnState == btnState;
                      });

        if (commands.empty())
        {
            m_CommandsMap.erase(button);
        }
    }

    ////////////////////////
    // Controller
    ////////////////////////

    Controller::Controller(unsigned long idx) :
        m_pImpl{ new ControllerImpl(idx) }
    {}

    Controller::~Controller() { delete m_pImpl; }

    void Controller::ProcessInput() { m_pImpl->ProcessInput(); }

    void Controller::AddCommand(Command& pCommand,
                                unsigned int button,
                                ButtonState btnState)
    {
        m_pImpl->AddCommand(pCommand, button, btnState);
    }

    void Controller::RemoveCommand(Command& pCommand,
                                   unsigned int button,
                                   ButtonState btnState)
    {
        m_pImpl->RemoveCommand(pCommand, button, btnState);
    }
}
