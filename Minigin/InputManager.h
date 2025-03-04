#pragma once
#include "Singleton.h"
#include "SceneManager.h"

namespace dae
{
    class InputManager final : public Singleton<InputManager>
    {
    public:
        bool ProcessInput();
    };
}
