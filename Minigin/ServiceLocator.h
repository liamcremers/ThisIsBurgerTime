#pragma once
#include "Singleton.h"
#include "SoundSystem.h"
#include <memory>
#include <filesystem>

namespace dae
{
    class ServiceLocator final : public Singleton<ServiceLocator>
    {
    public:
        static void RegisterSoundSystem(
            std::unique_ptr<SoundSystem>&& pSoundSystem);
        [[nodiscard]] static auto GetSoundSystem() -> SoundSystem&;

    private:
        friend class Singleton<ServiceLocator>;
        ServiceLocator() = default;

        static std::unique_ptr<SoundSystem> m_pSoundSystem;
    };
}