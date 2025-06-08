#pragma once
enum class PlayerInputKeys
{
    MoveLeft,
    MoveRight,
    MoveUp,
    MoveDown,
    Attack
#ifdef _DEBUG
        ,
    Die
#endif // _DEBUG
};