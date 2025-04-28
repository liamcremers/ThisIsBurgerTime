#include "SoundSystem.h"
#include "ServiceLocator.h"
#ifdef DEBUG_SOUND
#ifdef DEBUG_RENDER
#include "DebugRenderer.h"
#endif // DEBUG_RENDER
#endif // DEBUG_SOUND

#include <SDL_mixer.h>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <unordered_map>
#include <cassert>

class dae::SDLSoundSystem::SDLSoundImpl
{
    struct SoundRequest
    {
        SoundId id;
        float volume;
        bool repeat;
    };

public:
    explicit SDLSoundImpl(std::filesystem::path path);
    ~SDLSoundImpl();

    void Play(SoundId id, const float volume, bool doRepeat);
    void Load(SoundId id, const std::string& path);
    void Unload(SoundId id);
    void ProcessQueue(std::stop_token& st);
    auto GetSoundClip(SoundId id) -> Mix_Chunk*;

    SDLSoundImpl(const SDLSoundImpl&) = delete;
    SDLSoundImpl(SDLSoundImpl&&) = delete;
    auto operator=(const SDLSoundImpl&) -> SDLSoundImpl& = delete;
    auto operator=(SDLSoundImpl&&) -> SDLSoundImpl& = delete;

private:
    std::mutex m_QueueMutex;
    std::queue<SoundRequest> m_SoundQueue;
    std::unordered_map<SoundId, Mix_Chunk*> m_SoundMap;
    std::jthread m_SoundThread;
    std::condition_variable_any m_SoundRequested_CV;

    std::filesystem::path m_DataPath;
};

dae::SDLSoundSystem::SDLSoundImpl::SDLSoundImpl(std::filesystem::path path)
{
    Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3 | MIX_INIT_WAVPACK);
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,
                      MIX_DEFAULT_FORMAT,
                      MIX_DEFAULT_CHANNELS,
                      MIX_MAX_VOLUME) == -1)
    {
        throw std::runtime_error("Failed to initialize SDL audio");
    }
    m_SoundThread =
        std::jthread([this](std::stop_token st) { ProcessQueue(st); });
    m_DataPath = path;
}

dae::SDLSoundSystem::SDLSoundImpl::~SDLSoundImpl()
{
    m_SoundThread.request_stop();
    m_SoundRequested_CV.notify_one();

    Mix_HaltChannel(-1);
    for (auto& [id, chunk] : m_SoundMap)
    {
        Mix_FreeChunk(chunk);
    }
    Mix_CloseAudio();
    Mix_Quit();
}

void dae::SDLSoundSystem::SDLSoundImpl::Play(SoundId id,
                                             const float volume,
                                             bool doRepeat)
{
    {
        std::lock_guard lock{ m_QueueMutex };
        if (m_SoundMap.contains(id))
            m_SoundQueue.push({ id, volume, doRepeat });
        else
            assert("Sound not loaded");
    }
    m_SoundRequested_CV.notify_one();
}

void dae::SDLSoundSystem::SDLSoundImpl::Load(SoundId id,
                                             const std::string& path)
{
    std::lock_guard lock{ m_QueueMutex };
    if (!m_SoundMap.contains(id))
        if (auto* chunk = Mix_LoadWAV((m_DataPath / path).string().c_str()))
            m_SoundMap[id] = chunk;
}

void dae::SDLSoundSystem::SDLSoundImpl::Unload(SoundId id)
{
    std::lock_guard lock{ m_QueueMutex };
    if (m_SoundMap.contains(id))
    {
        Mix_FreeChunk(m_SoundMap[id]);
        m_SoundMap.erase(id);
    }
}

void dae::SDLSoundSystem::SDLSoundImpl::ProcessQueue(std::stop_token& st)
{
    while (!st.stop_requested())
    {
        SoundRequest req{};
        std::lock_guard lock{ m_QueueMutex };
        if (!m_SoundQueue.empty())
        {
            req = m_SoundQueue.front();
            m_SoundQueue.pop();

            auto [id, volume, repeat] = req;
            if (m_SoundMap.contains(id))
            {
                const int channel =
                    Mix_PlayChannel(-1, m_SoundMap[id], repeat ? -1 : 0);
                Mix_Volume(channel, static_cast<int>(volume * MIX_MAX_VOLUME));
            }
        }
        else
        {
            m_SoundRequested_CV.wait(m_QueueMutex);
        }
    }
}

[[nodiscard]]
auto dae::SDLSoundSystem::SDLSoundImpl::GetSoundClip(SoundId id) -> Mix_Chunk*
{
    return m_SoundMap[id];
}

// SDLSoundSystem
dae::SDLSoundSystem::SDLSoundSystem(std::filesystem::path path) :
    m_pImpl{ new SDLSoundImpl(path) }
{}

void dae::SDLSoundSystem::Play(SoundId id, const float volume, bool doRepeat)
{
    m_pImpl->Play(id, volume, doRepeat);

#ifdef DEBUG_SOUND
    dae::DebugRenderer::GetInstance().RenderText("Playing sound: " +
                                                 std::to_string(id));
    if (!m_pImpl->GetSoundClip(id))
        dae::DebugRenderer::GetInstance().RenderText("Sound not loaded: " +
                                                     std::to_string(id));
#endif // DEBUG_SOUND
}

void dae::SDLSoundSystem::Load(SoundId id, const std::string& path)
{
    m_pImpl->Load(id, path);

#ifdef DEBUG_SOUND
    dae::DebugRenderer::GetInstance().RenderText("Loading sound: " +
                                                 std::to_string(id));
#endif // DEBUG_SOUND
}

void dae::SDLSoundSystem::Unload(SoundId id)
{
    m_pImpl->Unload(id);

#ifdef DEBUG_SOUND
    dae::DebugRenderer::GetInstance().RenderText("Unloading sound: " +
                                                 std::to_string(id));
#endif // DEBUG_SOUND
}
