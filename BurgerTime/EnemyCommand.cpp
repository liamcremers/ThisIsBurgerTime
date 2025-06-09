#include "EnemyCommand.h"

EnemyCommand::EnemyCommand(dae::GameObject& pGameObject, EnemyInputKeys input) :
    GameObjectCommand{ &pGameObject },
    m_Input{ input }
{}

void EnemyCommand::Execute() { m_pEnemyComponent->HandleInput(m_Input); }

void EnemyCommand::Undo() {}
