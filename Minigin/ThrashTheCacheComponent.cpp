#include "ThrashTheCacheComponent.h"
#include "imgui.h"
#include <utility>

dae::ThrashTheCacheComponent::ThrashTheCacheComponent(GameObject& owner, int windowWidth, int windowHeigth)
	: BaseComponent(owner),
	m_WindowWidth(windowWidth),
	m_WindowHeight(windowHeigth)
{}

void dae::ThrashTheCacheComponent::RenderExercise1Window() const
{
	ImGui::Begin("Exercise 1");
	ImGui::Text("This is the Exercise 1 window.");

	if (ImGui::Button("-")) m_SamplesExercise1 = std::max(1, m_SamplesExercise1 - 1);
	ImGui::SameLine();
	ImGui::Text("Samples: %d", m_SamplesExercise1);
	ImGui::SameLine();
	if (ImGui::Button("+")) m_SamplesExercise1 += 1;

	if (ImGui::Button("Thrash the Cache"))
	{
	}

	ImGui::End();
}

void dae::ThrashTheCacheComponent::RenderExercise2Window() const
{
	ImGui::Begin("Exercise 2");
	ImGui::Text("This is the Exercise 2 window.");

	if (ImGui::Button("-")) m_SamplesExercise2 = std::max(1, m_SamplesExercise2 - 1);
	ImGui::SameLine();
	ImGui::Text("Samples: %d", m_SamplesExercise2);
	ImGui::SameLine();
	if (ImGui::Button("+")) m_SamplesExercise2 += 1;

	if (ImGui::Button("Thrash the Cache with GameObject3D"))
	{
	}

	if (ImGui::Button("Thrash the Cache with GameObject3DAlt"))
	{
	}

	ImGui::End();
}

void dae::ThrashTheCacheComponent::RenderUI() const
{
	RenderExercise1Window();
	RenderExercise2Window();
}
