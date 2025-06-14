#pragma once
#include "Singleton.h"

enum class GameModeType
{
    SinglePlayer,
    TwoPlayer,
    VSMode
};

class GameMode final : public dae::Singleton<GameMode>
{
public:
    void SetGameMode(GameModeType mode);

    GameModeType GetGameMode() const;

    bool IsSinglePlayer() const;

    bool IsTwoPlayer() const;

    bool IsVSMode() const;

private:
    friend class Singleton<GameMode>;
    GameMode() = default;

    GameModeType m_CurrentMode = GameModeType::SinglePlayer;
};
