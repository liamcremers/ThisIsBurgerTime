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

        std::unique_ptr<Controller> m_pController{};
        std::unique_ptr<MoveCommand> m_pMoveCommandUp{};
        std::unique_ptr<MoveCommand> m_pMoveCommandDown{};
        std::unique_ptr<MoveCommand> m_pMoveCommandLeft{};
        std::unique_ptr<MoveCommand> m_pMoveCommandRight{};
    };
}
