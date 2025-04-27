#include "BurgerPartComponent.h"

#include "BurgerTimeLayers.h"
#include "BurgerTimeSoundIds.h"
#include <GameObject.h>
#include <ColliderComponent.h>
#include <ServiceLocator.h>

BurgerPartComponent::BurgerPartComponent(dae::GameObject& parent) :
    BaseComponent{ parent }
{
    if (auto* collider = parent.GetComponent<dae::ColliderComponent>())
    {
        collider->SubscribeToBeginOverlap(
            [this](const dae::ColliderComponent& other)
            { OnBeginOverlap(other); });
    }
}

void BurgerPartComponent::OnBeginOverlap(const dae::ColliderComponent& other)
{
    static constexpr float SOUND_VOLUME = 0.4f;
    if ((other.GetLayer() & static_cast<uint16_t>(CollisionLayer::Player)) != 0)
    {
        dae::ServiceLocator::GetInstance().GetSoundSystem().Play(
            SoundIds::BurgerStep, SOUND_VOLUME);
    }
}