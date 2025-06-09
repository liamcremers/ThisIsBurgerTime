#pragma once
#include "EnemyComponent.h"

#include <GameObject.h>
#include <Command.h>

class EnemyCommand : public dae::GameObjectCommand
{
public:
    EnemyCommand(dae::GameObject& pGameObject, EnemyInputKeys input);
    void Execute() override;
    void Undo() override;

private:
    EnemyInputKeys m_Input{};
    EnemyComponent* m_pEnemyComponent = [&]()
    {
        auto playerComp = GetGameObject()->GetComponent<EnemyComponent>();
        assert(playerComp && "EnemyCommand depends on EnemyComponent");
        return playerComp;
    }();
};
