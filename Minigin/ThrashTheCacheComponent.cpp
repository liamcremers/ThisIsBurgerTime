#include "ThrashTheCacheComponent.h"
#include "imgui.h"
#include "imgui_plot.h"
#include <iostream>
#define NOMINMAX
#include <Windows.h>

dae::ThrashTheCacheComponent::ThrashTheCacheComponent(GameObject& owner, int windowWidth, int windowHeight)
	: BaseComponent(owner),
	m_WindowWidth(windowWidth),
	m_WindowHeight(windowHeight)
{}
TimingsVec dae::ThrashTheCacheComponent::PlotAverage(std::function<TimingsVec()> exercise, int AmountOfTest)
{
	std::vector<TimingsVec> results(AmountOfTest);
	std::ranges::generate(results, exercise);

	results.erase(results.begin());
	results.pop_back();

	TimingsVec averageTimings(results[0].size(), 0);
	for (const TimingsVec& result : results)
	{
		std::ranges::transform(averageTimings, result, averageTimings.begin(), std::plus<>{});
	}
	for (auto& time : averageTimings)
		time /= static_cast<float>(results.size());

	return averageTimings;
}


void dae::ThrashTheCacheComponent::RenderExercise1Window()
{
	ImGui::Begin("Exercise 1");

	if (ImGui::Button("-")) m_SamplesExercise1 = std::max(1, m_SamplesExercise1 - 1);
	ImGui::SameLine();
	ImGui::Text("Samples: %d", m_SamplesExercise1);
	ImGui::SameLine();
	if (ImGui::Button("+")) m_SamplesExercise1 += 1;

	if (ImGui::Button("Thrash the Cache"))
	{
		m_TimingsExercise1 = PlotAverage(Exercise1, m_SamplesExercise1);
	}

	// Plot the results if timings are available
	if (!m_TimingsExercise1.empty())
	{
		ImGui::PlotConfig conf;
		conf.values.ys = m_TimingsExercise1.data();
		conf.values.count = static_cast<int>(m_TimingsExercise1.size());
		conf.scale.min = 0;
		conf.scale.max = static_cast<float>(*std::max_element(m_TimingsExercise1.begin(), m_TimingsExercise1.end()));
		conf.tooltip.show = true;
        conf.tooltip.format = "x=%.0f, y=%.2f"; 
		conf.grid_x.show = true;
		conf.grid_y.show = true;
		conf.frame_size = ImVec2(200, 100);
		conf.line_thickness = 2.f;
		conf.values.color = ImColor(255, 165, 0);

		ImGui::Plot("Exercise 1 Timings", conf);
	}

	ImGui::End();
}

void dae::ThrashTheCacheComponent::RenderExercise2Window()
{
    ImGui::Begin("Exercise 2");

    if (ImGui::Button("-")) m_SamplesExercise2 = std::max(1, m_SamplesExercise2 - 1);
    ImGui::SameLine();
    ImGui::Text("Samples: %d", m_SamplesExercise2);
    ImGui::SameLine();
    if (ImGui::Button("+")) m_SamplesExercise2 += 1;

    if (ImGui::Button("Thrash the Cache with GameObject3D"))
    {
        m_TimingsExercise2GameObject3D = PlotAverage(Exercise2<GameObject3D>, m_SamplesExercise2);
    }

    if (ImGui::Button("Thrash the Cache with GameObject3DAlt"))
    {
        m_TimingsExercise2GameObject3DAlt = PlotAverage(Exercise2<GameObject3DAlt>, m_SamplesExercise2);
    }

    if (!m_TimingsExercise2GameObject3D.empty())
    {
        ImGui::PlotConfig conf;
        conf.values.ys = m_TimingsExercise2GameObject3D.data();
        conf.values.count = static_cast<int>(m_TimingsExercise2GameObject3D.size());
        conf.scale.min = 0;
        conf.scale.max = static_cast<float>(*std::max_element(m_TimingsExercise2GameObject3D.begin(), m_TimingsExercise2GameObject3D.end()));
        conf.tooltip.show = true;
        conf.tooltip.format = "x=%.0f, y=%.2f"; 
        conf.grid_x.show = true;
        conf.grid_y.show = true;
        conf.frame_size = ImVec2(200, 100);
        conf.line_thickness = 2.f;
        conf.values.color = ImColor(0, 191, 255);

        ImGui::Plot("GameObject3D Timings", conf);
    }

    if (!m_TimingsExercise2GameObject3DAlt.empty())
    {
        ImGui::PlotConfig conf;
        conf.values.ys = m_TimingsExercise2GameObject3DAlt.data();
        conf.values.count = static_cast<int>(m_TimingsExercise2GameObject3DAlt.size());
        conf.scale.min = 0;
        conf.scale.max = static_cast<float>(*std::max_element(m_TimingsExercise2GameObject3DAlt.begin(), m_TimingsExercise2GameObject3DAlt.end()));
        conf.tooltip.show = true;
        conf.tooltip.format = "x=%.0f, y=%.2f"; 
        conf.grid_x.show = true;
        conf.grid_y.show = true;
        conf.frame_size = ImVec2(200, 100);
        conf.line_thickness = 2.f;
        conf.values.color = ImColor(0, 191, 0);

        ImGui::Plot("GameObject3DAlt Timings", conf);
    }

    if (!m_TimingsExercise2GameObject3D.empty() &&
        !m_TimingsExercise2GameObject3DAlt.empty())
    {
        ImGui::Text("Combined:");
        ImGui::PlotConfig conf;
        const float* ys_list[2] = { m_TimingsExercise2GameObject3D.data(), m_TimingsExercise2GameObject3DAlt.data() };
        conf.values.ys_list = ys_list;
        conf.values.ys_count = 2; // Two lines
        conf.values.count = static_cast<int>(m_TimingsExercise2GameObject3D.size()); // Same size for both datasets
        conf.scale.min = 0;
        conf.scale.max = std::max(
            *std::max_element(m_TimingsExercise2GameObject3D.begin(), m_TimingsExercise2GameObject3D.end()),
            *std::max_element(m_TimingsExercise2GameObject3DAlt.begin(), m_TimingsExercise2GameObject3DAlt.end())
        );
        conf.tooltip.show = true;
        conf.tooltip.format = "x=%.0f, y=%.2f"; 
        conf.grid_x.show = true;
        conf.grid_y.show = true;
        conf.frame_size = ImVec2(300, 150);
        conf.line_thickness = 2.f;
        const auto colors = std::make_unique<ImU32[]>(2);
        colors[0] = ImColor(0, 191, 255);
        colors[1] = ImColor(0, 191, 0);
        conf.values.colors = colors.get();
        ImGui::Plot("Combined Timings", conf);
    }
    ImGui::End();
}
void dae::ThrashTheCacheComponent::RenderUI()
{
	RenderExercise1Window();
	RenderExercise2Window();
}