#pragma once
#include <string>
#include <functional>
#include <filesystem>

struct SDL_Window;

namespace dae
{
    class Minigin final
    {
    public:
        explicit Minigin(const std::filesystem::path& dataPath);
        ~Minigin();
        void Run(const std::function<void()>& load);
        void GameLoop();

        [[nodiscard]] static consteval int GetWindowWidth()
        {
            return m_WindowWidth;
        }

        [[nodiscard]] static consteval int GetWindowHeight()
        {
            return m_WindowHeight;
        }

        Minigin(const Minigin& other) = delete;
        Minigin(Minigin&& other) = delete;
        Minigin& operator=(const Minigin& other) = delete;
        Minigin& operator=(Minigin&& other) = delete;

    private:
        static constexpr int m_msPerFrame{ 16 };
        static constexpr int m_WindowWidth{ 512 };
        static constexpr int m_WindowHeight{ 480 };

        SDL_Window* m_window{};
        float m_lag{};
        bool m_continue{ true };
    };
}