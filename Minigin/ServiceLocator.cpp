#include "ServiceLocator.h"
#include "SoundSystem.h"
#include <cassert>

std::filesystem::path dae::ServiceLocator::m_Datapath;
std::unique_ptr<dae::SoundSystem> dae::ServiceLocator::m_pSoundSystem = nullptr;

void dae::ServiceLocator::Init(const std::filesystem::path& dataPath)
{
    m_Datapath = dataPath;
}

auto dae::ServiceLocator::GetSoundSystem() -> SoundSystem&
{
    return *m_pSoundSystem;
}

void dae::ServiceLocator::RegisterSoundSystem(
    std::unique_ptr<SoundSystem>&& pSoundSystem)
{
    m_pSoundSystem = pSoundSystem ? std::move(pSoundSystem) :
                                    std::make_unique<NullSoundSystem>();
}

auto dae::ServiceLocator::GetDataPath() -> const std::filesystem::path&
{
    return m_Datapath;
}
