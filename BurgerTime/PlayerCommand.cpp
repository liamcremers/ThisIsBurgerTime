#include "PlayerCommand.h"

PlayerCommand::PlayerCommand(dae::GameObject& pGameObject,
                             PlayerInputKeys input) :
    GameObjectCommand{ &pGameObject },
    m_Input{ input }
{}

void PlayerCommand::Execute() { m_pPlayerComponent->HandleInput(m_Input); }

void PlayerCommand::Undo() {}
