#include "BurgerState.h"
#include "BurgerPartComponent.h"
#include "BurgerTimeLayers.h"

#include "BurgerTimeSoundIds.h"
#include <ServiceLocator.h>

#include <GameObject.h>
#include <BaseComponent.h>
#include <glm.hpp>

using namespace BurgerComp;
using namespace BurgerStates;
static constexpr float SOUND_VOLUME = 0.4f;
static constexpr auto PLAY_SOUND = [](SoundIds id, float volume)
{ dae::ServiceLocator::GetInstance().GetSoundSystem().Play(id, volume); };

// IDLE STATE
void BurgerStates::IdleState::Enter(BurgerPartComponent& part)
{
    part.OnIdle();
}

auto BurgerStates::IdleState::Update(BurgerPartComponent& part) -> BurgerState&
{
    if (part.GetOverlaps().contains(CollisionLayer::Player))
        return part.GetWalkedOnState();
    return *this;
}

void BurgerStates::WalkedOnState::Enter(BurgerPartComponent& part)
{
    part.OnWalkedOn();
    PLAY_SOUND(SoundIds::BurgerStep, SOUND_VOLUME);
}

// WALKED ON STATE
auto BurgerStates::WalkedOnState::Update(BurgerPartComponent& part)
    -> BurgerState&
{
    if (part.HasAllPartsWalkedOn())
        return part.GetFallingState();
    return *this;
}

// FALLING STATE
void BurgerStates::FallingState::Enter(BurgerPartComponent& part)
{
    (part.HasEnemyOnTop()) ? m_FallAmount = 3 : m_FallAmount = 1;

    part.OnFalling();
}

auto BurgerStates::FallingState::Update(BurgerPartComponent& part)
    -> BurgerState&
{
    if (part.GetBeginOverlaps().contains(CollisionLayer::BurgerPlate))
        return part.GetIdleState();

    if (part.GetBeginOverlaps().contains(CollisionLayer::Floor))
    {
        if (m_FallAmount <= 0)
            return part.GetIdleState();
        m_FallAmount--;
    }

    return *this;
}
