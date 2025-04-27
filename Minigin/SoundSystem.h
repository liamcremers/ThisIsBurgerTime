#pragma once
#include "ResourceManager.h"

#include <vector>
#include <memory>
#include <string>
#include <filesystem>

namespace dae
{
    using SoundId = unsigned short;

    class SoundSystem
    {
    public:
        virtual ~SoundSystem() = default;
        virtual void Play(SoundId id,
                          const float volume = 1.f,
                          bool doRepeat = false) = 0;
        virtual void Load(SoundId id, const std::string& path) = 0;
        virtual void Unload(SoundId id) = 0;
        [[nodiscard]]
        virtual auto GetDataPath() const -> std::filesystem::path final;
        virtual void SetDataPath(const std::filesystem::path& dataPath) final;

    private:
        std::filesystem::path m_DataPath;
    };

    class NullSoundSystem final : public SoundSystem
    {
    public:
        void Play(SoundId, const float, bool) override {};
        void Load(SoundId, const std::string&) override {};
        void Unload(SoundId) override{};
    };

    class SDLSoundSystem final : public SoundSystem
    {
    public:
        SDLSoundSystem();
        void Play(SoundId id, const float volume, bool doRepeat) override;
        void Load(SoundId id, const std::string& path) override;
        void Unload(SoundId id) override;

    private:
        class SDLSoundImpl;
        SDLSoundImpl* m_pImpl;
    };
}