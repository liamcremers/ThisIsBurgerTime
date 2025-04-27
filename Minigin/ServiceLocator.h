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
        static void Init(const std::filesystem::path& dataPath);
        static auto GetSoundSystem() -> SoundSystem&;
        static void RegisterSoundSystem(
            std::unique_ptr<SoundSystem>&& pSoundSystem);
        static auto GetDataPath() -> const std::filesystem::path&;

    private:
        friend class Singleton<ServiceLocator>;
        ServiceLocator() = default;
        static std::unique_ptr<SoundSystem> m_pSoundSystem;
        static std::filesystem::path m_Datapath;
    };
}