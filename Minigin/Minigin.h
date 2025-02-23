#pragma once
#include <string>
#include <functional>
#include <filesystem>

namespace dae
{
	class Minigin final
	{
		bool m_continue{true};
		float m_lag{};
		const int m_msPerFrame{ 16 };
		const int m_WindowWidth{ 640 };
		const int m_WindowHeight{ 480 };
	public:
		explicit Minigin(const std::filesystem::path& dataPath);
		~Minigin();
		void Run(const std::function<void()>& load);
		void GameLoop();
		[[nodiscard]] constexpr auto GetWindowWidth() const -> int { return m_WindowWidth; }
		[[nodiscard]] constexpr auto GetWindowHeight() const -> int { return m_WindowHeight; }

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;
	};
}