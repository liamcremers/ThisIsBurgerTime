#pragma once
#include "Singleton.h"
#include <chrono>

namespace dae
{
    using Clock = std::chrono::high_resolution_clock;

    class EngineTime : public Singleton<EngineTime>
    {
    public:
        void Update();
        [[nodiscard]] auto GetDeltaTime() const -> float;
        [[nodiscard]] auto GetFixedTimeStep() const -> float;
        [[nodiscard]] auto GetLastTime() const -> const Clock::time_point&;

    private:
        friend class Singleton<EngineTime>;
        EngineTime() = default;

        static constexpr float m_FixedTimeStep{ 1.0f / 60.0f };

        Clock::time_point m_LastTime{ Clock::now() };
        float m_DeltaTime{};
    };
}
