#include "Command.h"
#include "GameObject.h"
#include "EngineTime.h"

dae::GameObjectCommand::GameObjectCommand(GameObject* pGameObject) :
    m_pGameObject{ pGameObject }
{}

auto dae::GameObjectCommand::GetGameObject() const -> dae::GameObject*
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

void dae::MoveCommand::Execute()
{
    auto& worldPos = GetGameObject()->GetWorldPosition();
    auto pos = worldPos + glm::vec2{
        m_Direction.x * (m_Speed * EngineTime::GetInstance().GetDeltaTime()),
        m_Direction.y * (m_Speed * EngineTime::GetInstance().GetDeltaTime())
    };
    GetGameObject()->SetLocalPosition(pos);
}

void dae::MoveCommand::SetSpeed(int speed) { m_Speed = speed; }
