#pragma once
#include <glm.hpp>

namespace dae
{
    class Command
    {
    public:
        virtual void Execute() = 0;
        virtual void Undo() = 0;

        Command() = default;
        virtual ~Command() = default;
        Command(const Command& other) noexcept = default;
        Command(Command&& other) noexcept = default;
        Command& operator=(const Command& other) noexcept = default;
        Command& operator=(Command&& other) noexcept = default;
    };

    class GameObject;

    class GameObjectCommand : public Command
    {
    public:
        GameObjectCommand(GameObject* pGameObject);
        ~GameObjectCommand() = default;
        void Execute() override = 0;
        void Undo() override = 0;

    protected:
        virtual GameObject* GetGameObject() const final;

    private:
        GameObject* m_pGameObject{};
    };

    class MoveCommand : public GameObjectCommand
    {
    public:
        MoveCommand(GameObject& pGameObject,
                    glm::i8vec2 direction,
                    int speed = 100);
        ~MoveCommand() = default;
        void Execute() override;
        void Undo() override;
        void SetSpeed(int speed);

    private:
        glm::i8vec2 m_Direction{};
        int m_Speed{ 100 };
    };
}
