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
        virtual void Play(SoundId id,
                          const float volume = 1.f,
                          bool doRepeat = false) = 0;
        virtual void Load(SoundId id, const std::string& path) = 0;
        virtual void Unload(SoundId id) = 0;

        SoundSystem() = default;
        virtual ~SoundSystem() = default;
        SoundSystem(const SoundSystem&) = delete;
        SoundSystem(SoundSystem&&) = delete;
        SoundSystem& operator=(const SoundSystem&) = delete;
        SoundSystem& operator=(SoundSystem&&) = delete;
    };

    class NullSoundSystem final : public SoundSystem
    {
    public:
        void Play(SoundId, const float, bool) override {};
        void Load(SoundId, const std::string&) override {};
        void Unload(SoundId) override{};

        NullSoundSystem() = default;
        virtual ~NullSoundSystem() = default;
        NullSoundSystem(const NullSoundSystem&) = delete;
        NullSoundSystem(NullSoundSystem&&) = delete;
        NullSoundSystem& operator=(const NullSoundSystem&) = delete;
        NullSoundSystem& operator=(NullSoundSystem&&) = delete;
    };

    class SDLSoundSystem final : public SoundSystem
    {
    public:
        explicit SDLSoundSystem(std::filesystem::path path);
        void Play(SoundId id, const float volume, bool doRepeat) override;
        void Load(SoundId id, const std::string& path) override;
        void Unload(SoundId id) override;

        SDLSoundSystem() = delete;
        virtual ~SDLSoundSystem() = default;
        SDLSoundSystem(const SDLSoundSystem&) = delete;
        SDLSoundSystem(SDLSoundSystem&&) = delete;
        SDLSoundSystem& operator=(const SDLSoundSystem&) = delete;
        SDLSoundSystem& operator=(SDLSoundSystem&&) = delete;

    private:
        class SDLSoundImpl;
        SDLSoundImpl* m_pImpl;
    };
}