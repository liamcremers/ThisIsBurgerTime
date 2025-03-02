#include "ThrashTheCacheComponent.h"
#include "imgui.h"

dae::ThrashTheCacheComponent::ThrashTheCacheComponent(GameObject& owner)
	: BaseComponent(owner)
{}

void dae::ThrashTheCacheComponent::RenderUI() const
{
	ImGui::Begin("Thrash the Cache");

	// Display the image or any other ImGui widgets
	// Assuming you have loaded the image texture somewhere else
	// ImGui::Image((void*)textureID, ImVec2(width, height));

	ImGui::Text("This is the Thrash the Cache component.");

	ImGui::End();
}