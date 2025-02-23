#include "FPSComponent.h"
#include "GameObject.h"

dae::FPSComponent::FPSComponent(GameObject& parent)
	: BaseComponent(parent)
{
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	GetOwner()->AddComponent<TextComponent>("FPS: ",font);
}

void dae::FPSComponent::Update()
{
	GetOwner()->GetComponent<TextComponent>()->SetText("FPS: " + std::to_string(int(1.f / dae::EngineTime::GetInstance().GetDeltaTime())));
}
