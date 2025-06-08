#pragma once
#include "PlayerComponent.h"

#include <GameObject.h>
#include <Command.h>

class PlayerCommand : public dae::GameObjectCommand
{
public:
    PlayerCommand(dae::GameObject& pGameObject, PlayerInputKeys input);
    void Execute() override;
    void Undo() override;

private:
    PlayerInputKeys m_Input{};
    PlayerComponent* m_pPlayerComponent = [&]()
    {
        auto playerComp = GetGameObject()->GetComponent<PlayerComponent>();
        assert(playerComp && "PlayerCommand depends on PlayerComponent");
        return playerComp;
    }();
};
