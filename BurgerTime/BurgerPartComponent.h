#pragma once
#include <BaseComponent.h>

namespace dae
{
    class ColliderComponent;
}

class BurgerPartComponent final : public dae::BaseComponent
{
public:
    BurgerPartComponent(dae::GameObject& parent);
    void OnBeginOverlap(const dae::ColliderComponent& other);
};
