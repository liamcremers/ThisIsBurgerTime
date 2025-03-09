#include "Command.h"
#include "GameObject.h"

dae::GameObjectCommand::GameObjectCommand(GameObject* pGameObject) :
    m_pGameObject{ pGameObject }
{}

dae::GameObject* dae::GameObjectCommand::GetGameObject() const
{
    return m_pGameObject;
}

dae::MoveCommand::MoveCommand(GameObject& pGameObject,
                              glm::i8vec2 direction,
                              int speed) :
    GameObjectCommand{ &pGameObject },
    m_Direction{ direction },
    m_Speed{ speed }
{}

void dae::MoveCommand::Execute() {}

void dae::MoveCommand::SetSpeed(int speed) { m_Speed = speed; }
