#pragma once
#include "Singleton.h"
#include <chrono>

namespace dae
{
    class EngineTime : public Singleton<EngineTime>
    {
    public:
        void Update()
        {
            using namespace std::chrono;
            auto now = high_resolution_clock::now();
            m_DeltaTime = duration<float>(now - m_LastTime).count();
            m_LastTime = now;
        }

        float GetDeltaTime() const { return m_DeltaTime; }

        float GetFixedTimeStep() const { return m_FixedTimeStep; }

        auto GetLastTime() const { return m_LastTime; }

    private:
        EngineTime() :
            m_LastTime(std::chrono::high_resolution_clock::now()),
            m_DeltaTime(0.0f),
            m_FixedTimeStep(1.0f / 60.0f)
        {}
        friend class Singleton<EngineTime>;

        std::chrono::high_resolution_clock::time_point m_LastTime;
        float m_DeltaTime;
        float m_FixedTimeStep;
    };
}
