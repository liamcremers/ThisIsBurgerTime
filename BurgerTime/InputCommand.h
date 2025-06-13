#pragma once
#include "IControllable.h"
#include <Command.h>

class InputCommand : public dae::GameObjectCommand
{
public:
    InputCommand(dae::GameObject& obj,
                 InputKey input,
                 IControllable* controllable);

    void Execute() override;
    void Undo() override;

private:
    InputKey m_Input;
    IControllable* m_pControllable;
};
