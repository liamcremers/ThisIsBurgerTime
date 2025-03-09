#pragma once
#include "BaseComponent.h"
#include <memory>
#include <vector>
#include <glm.hpp>

namespace dae
{
    class Controller;
    class MoveCommand;

    class PlayerComponent final : public BaseComponent
    {
    public:
        PlayerComponent(GameObject& parent, unsigned long idx);
        ~PlayerComponent();
        const Controller* GetController() const;

        void SetSpeed(int speed);

    private:
        static constexpr glm::i8vec2 MoveUp{ 0, -1 };
        static constexpr glm::i8vec2 MoveDown{ 0, 1 };
        static constexpr glm::i8vec2 MoveLeft{ -1, 0 };
        static constexpr glm::i8vec2 MoveRight{ 1, 0 };

        static constexpr unsigned int XINPUT_GAMEPAD_DPAD_UP = 0x0001;
        static constexpr unsigned int XINPUT_GAMEPAD_DPAD_DOWN = 0x0002;
        static constexpr unsigned int XINPUT_GAMEPAD_DPAD_LEFT = 0x0004;
        static constexpr unsigned int XINPUT_GAMEPAD_DPAD_RIGHT = 0x0008;

        std::unique_ptr<Controller> m_pController{};
        std::unique_ptr<MoveCommand> m_pMoveCommandUp{};
        std::unique_ptr<MoveCommand> m_pMoveCommandDown{};
        std::unique_ptr<MoveCommand> m_pMoveCommandLeft{};
        std::unique_ptr<MoveCommand> m_pMoveCommandRight{};
    };
}
