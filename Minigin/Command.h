#pragma once
#include <glm.hpp>

namespace dae
{
    class Command
    {
    public:
        virtual void Execute() = 0;
        virtual void Undo() = 0;
    };

    class GameObject;

    class GameObjectCommand : public Command
    {
    public:
        GameObjectCommand(GameObject* pGameObject);
        void Execute() override = 0;
        void Undo() override = 0;

    protected:
        [[nodiscard]]
        virtual auto GetGameObject() const -> GameObject* final;

    private:
        GameObject* m_pGameObject{};
    };

    class MoveCommand : public GameObjectCommand
    {
    public:
        MoveCommand(GameObject& pGameObject,
                    glm::i8vec2 direction,
                    int speed = 100);
        void Execute() override;
        void Undo() override;
        void SetSpeed(int speed);

    private:
        glm::i8vec2 m_Direction{};
        int m_Speed{ 100 };
    };

}
