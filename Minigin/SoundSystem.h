#pragma once
#include "Singleton.h"

namespace dae
{
    class SoundSystem : public Singleton<SoundSystem>
    {
    public:
    private:
        friend class Singleton<SoundSystem>;
        SoundSystem() = default;
    };
}