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
        [[nodiscard]] auto GetLastTime() const
            -> const std::chrono::high_resolution_clock::time_point&;

    private:
        friend class Singleton<EngineTime>;
        EngineTime() = default;

        static constexpr float m_FixedTimeStep{ 1.0f / 60.0f };
        std::chrono::high_resolution_clock::time_point m_LastTime{
            std::chrono::high_resolution_clock::now()
        };
        float m_DeltaTime{};
    };
}
