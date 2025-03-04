#pragma once
#include "Singleton.h"
#include <chrono>

namespace dae
{
    class EngineTime : public Singleton<EngineTime>
    {
    public:
        void Update();
        float GetDeltaTime() const;
        float GetFixedTimeStep() const;
        auto GetLastTime() const
            -> const std::chrono::high_resolution_clock::time_point&;

    private:
        EngineTime();
        friend class Singleton<EngineTime>;

        std::chrono::high_resolution_clock::time_point m_LastTime;
        float m_DeltaTime;
        float m_FixedTimeStep;
    };
}
