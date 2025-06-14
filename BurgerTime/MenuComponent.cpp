#include "MenuComponent.h"
#include "GameMode.h"
#include <InputManager.h>
#include <SceneManager.h>
#include <EngineTime.h>
#include <RenderComponent.h>
#include <algorithm>
#include <SDL.h>

MenuComponent::MenuComponent(dae::GameObject& parent) :
    BaseComponent(parent)
{
    m_MenuImages = { "Player1.png", "2Players.png", "Versus.png" };
    m_GameModes = { GameModeType::SinglePlayer,
                    GameModeType::TwoPlayer,
                    GameModeType::VSMode };
}

void MenuComponent::HandleInput(InputKey input)
{
    if (m_InputCooldown > 0.0f)
    {
        m_InputCooldown -= dae::EngineTime::GetInstance().GetDeltaTime();
        return;
    }

    switch (input)
    {
    case InputKey::MoveLeft:
    case InputKey::MoveUp:
        CycleUp();
        m_InputCooldown = INPUT_DELAY;
        break;
    case InputKey::MoveRight:
    case InputKey::MoveDown:
        CycleDown();
        m_InputCooldown = INPUT_DELAY;
        break;
    case InputKey::Attack:
        SelectOption();
        break;
    }
}

void MenuComponent::Update() { UpdateMenuImage(); }

void MenuComponent::SetRenderer(dae::RenderComponent* renderer)
{
    m_pRenderer = renderer;
}

void MenuComponent::CycleUp()
{
    rotate(
        m_MenuImages.rbegin(), m_MenuImages.rbegin() + 1, m_MenuImages.rend());
    rotate(m_GameModes.rbegin(), m_GameModes.rbegin() + 1, m_GameModes.rend());
    UpdateMenuImage();
}

void MenuComponent::CycleDown()
{
    rotate(m_MenuImages.begin(), m_MenuImages.begin() + 1, m_MenuImages.end());
    rotate(m_GameModes.begin(), m_GameModes.begin() + 1, m_GameModes.end());
    UpdateMenuImage();
}

void MenuComponent::SelectOption()
{
    GameMode::GetInstance().SetGameMode(m_GameModes[0]);
    dae::SceneManager::GetInstance().ActivateScene("Level0");

    switch (m_GameModes.front())
    {
    case GameModeType::SinglePlayer:
        dae::SceneManager::GetInstance().ActivateScene("SinglePlayer");
        break;
    case GameModeType::TwoPlayer:
        dae::SceneManager::GetInstance().ActivateScene("TwoPlayer");
        break;
    case GameModeType::VSMode:
        dae::SceneManager::GetInstance().ActivateScene("VSMode");
        break;
    default:
        break;
    }
    dae::SceneManager::GetInstance().DeactivateScene("Menu");
}

void MenuComponent::UpdateMenuImage()
{
    if (m_pRenderer)
    {
        m_pRenderer->SetTexture(m_MenuImages[0]);
        m_pRenderer->Scale(2.0f);
    }
}
