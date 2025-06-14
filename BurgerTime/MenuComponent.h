#pragma once
#include "BaseComponent.h"
#include "GameMode.h"
#include "IControllable.h"
#include <vector>
#include <string>

namespace dae
{
    class RenderComponent;
    class InputManager;
}

class MenuComponent : public dae::BaseComponent, public IControllable
{
public:
    MenuComponent(dae::GameObject& parent);

    void Update() override;

    void SetRenderer(dae::RenderComponent* renderer);
    void HandleInput(InputKey input) override;

private:
    void CycleUp();
    void CycleDown();
    void SelectOption();
    void UpdateMenuImage();



    std::vector<std::string> m_MenuImages{};
    std::vector<GameModeType> m_GameModes{};
    dae::RenderComponent* m_pRenderer{};
    int m_CurrentIndex{};
    float m_InputCooldown{};
    static constexpr float INPUT_DELAY = 0.1f;
};
