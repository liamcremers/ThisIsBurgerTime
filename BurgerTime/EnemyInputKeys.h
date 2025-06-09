#pragma once
enum class EnemyInputKeys
{
    MoveLeft,
    MoveRight,
    MoveUp,
    MoveDown
#ifdef _DEBUG
        ,
    Attack,
    Die
#endif // _DEBUG
};