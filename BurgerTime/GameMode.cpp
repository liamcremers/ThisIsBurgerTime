#include "GameMode.h"

void GameMode::SetGameMode(GameModeType mode) { m_CurrentMode = mode; }

GameModeType GameMode::GetGameMode() const { return m_CurrentMode; }

bool GameMode::IsSinglePlayer() const
{
    return m_CurrentMode == GameModeType::SinglePlayer;
}

bool GameMode::IsTwoPlayer() const
{
    return m_CurrentMode == GameModeType::TwoPlayer;
}

bool GameMode::IsVSMode() const { return m_CurrentMode == GameModeType::VSMode; }
