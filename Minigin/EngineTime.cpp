#include "EngineTime.h"

void dae::EngineTime::Update()
{
    using namespace std::chrono;
    auto now = high_resolution_clock::now();
    m_DeltaTime = duration<float>(now - m_LastTime).count();
    m_LastTime = now;
}

auto dae::EngineTime::GetDeltaTime() const -> float { return m_DeltaTime; }

auto dae::EngineTime::GetFixedTimeStep() const -> float
{
    return m_FixedTimeStep;
}

auto dae::EngineTime::GetLastTime() const
    -> const std::chrono::high_resolution_clock::time_point&
{
    return m_LastTime;
}