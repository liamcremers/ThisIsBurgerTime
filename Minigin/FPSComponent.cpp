#include "FPSComponent.h"
#include "GameObject.h"

dae::FPSComponent::FPSComponent(GameObject& parent, Font& font)
	: BaseComponent(parent),
	m_pFont{ font },
	m_pTextComponent{ parent.AddComponent<TextComponent>("FPS: ", font) }
{}

void dae::FPSComponent::Update()
{
	static int frameCount = 0;
	static float elapsedTime = 0.0f;
	frameCount++;
	elapsedTime += EngineTime::GetInstance().GetDeltaTime();
	if (elapsedTime >= 1.0f)
	{
		int fps = frameCount / static_cast<int>(elapsedTime);
		m_pTextComponent->SetText("FPS: " + std::to_string(fps));
		frameCount = 0;
		elapsedTime = 0.0f;
	}
}