#include "InputCommand.h"

InputCommand::InputCommand(dae::GameObject& obj,
                           InputKey input,
                           IControllable* controllable) :
    dae::GameObjectCommand(&obj),
    m_Input(input),
    m_pControllable(controllable)
{}

void InputCommand::Execute()
{
    if (m_pControllable)
        m_pControllable->HandleInput(m_Input);
}

void InputCommand::Undo() {}
