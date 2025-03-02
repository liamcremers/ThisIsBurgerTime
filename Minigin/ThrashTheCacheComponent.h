#pragma once  
#include "BaseComponent.h"  
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <functional>

using TimingsVec = std::vector<float>;
namespace dae
{
	struct Transform
	{
		float matrix[16] = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1 };
	};
	class GameObject3D
	{
	public:
		Transform transform{};
		int ID{};
	};
	class GameObject3DAlt
	{
	public:
		Transform* transform{};
		int ID{};
	};
	class ThrashTheCacheComponent final : public BaseComponent
	{
	public:
		ThrashTheCacheComponent(GameObject& owner, int windowWidth, int windowHeigth);
		void RenderUI() override;
	private:
		void RenderExercise1Window();
		void RenderExercise2Window();

		TimingsVec PlotAverage(std::function<TimingsVec()> exercise, int AmountOfTest);
		static TimingsVec Exercise1()
		{
			TimingsVec timings{};
			std::vector<int> v(static_cast<size_t>(std::pow(2, 26)));

			for (size_t step = 1; step <= 1024; step *= 2)
			{
				const auto start = std::chrono::high_resolution_clock::now();
				for (size_t i = 0; i < v.size(); i += step)
				{
					v[i] *= 2;
				}
				const auto end = std::chrono::high_resolution_clock::now();
				const auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
				timings.emplace_back(static_cast<float>(total));
			}
			return timings;
		}

		template <typename T>
		static TimingsVec Exercise2()
		{
			TimingsVec timings{};
			std::vector<T> v(static_cast<size_t>(std::pow(2, 26)));

			for (size_t step = 1; step <= 1024; step *= 2)
			{
				const auto start = std::chrono::high_resolution_clock::now();
				for (size_t i = 0; i < v.size(); i += step)
				{
					v[i].ID *= 2;
				}
				const auto end = std::chrono::high_resolution_clock::now();
				const auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
				timings.emplace_back(static_cast<float>(total));
			}
			return timings;
		}

		TimingsVec m_TimingsExercise1{};
		TimingsVec	m_TimingsExercise2GameObject3D{};
		TimingsVec m_TimingsExercise2GameObject3DAlt{};

		int m_SamplesExercise1{ 10 };
		int m_SamplesExercise2{ 100 };

		int m_WindowWidth{};
		int m_WindowHeight{};
		int m_HalfWindowWidth{ m_WindowWidth / 2 };
	};
}
