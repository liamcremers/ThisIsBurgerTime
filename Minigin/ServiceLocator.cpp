#include "ServiceLocator.h"
#include "SoundSystem.h"
#include <cassert>

std::unique_ptr<dae::SoundSystem> dae::ServiceLocator::m_pSoundSystem =
    std::make_unique<NullSoundSystem>();

[[nodiscard]]
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
