#include "FPSComponent.h"
#include "GameObject.h"

dae::FPSComponent::FPSComponent(GameObject& parent)
	: BaseComponent(parent)
{
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	GetGameObject()->AddComponent<TextComponent>("FPS: ",font);
}

void dae::FPSComponent::Update()
{
	GetGameObject()->GetComponent<TextComponent>()->SetText("FPS: " + std::to_string(int(1.f / dae::EngineTime::GetInstance().GetDeltaTime())));
}
