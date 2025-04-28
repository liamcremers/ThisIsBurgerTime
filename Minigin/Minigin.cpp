#include <stdexcept>
#include <sstream>
#include <iostream>
#include <thread>

#if WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "ServiceLocator.h"
#include "EngineTime.h"
#include "CollisionSystem.h"
#include "DebugRenderer.h"

static void LogSDLVersion(const std::string& message, const SDL_version& v)
{
#if WIN32
    std::stringstream ss;
    ss << message << (int)v.major << "." << (int)v.minor << "." << (int)v.patch
       << "\n";
    OutputDebugString(ss.str().c_str());
#else
    std::cout << message << (int)v.major << "." << (int)v.minor << "."
              << (int)v.patch << "\n";
#endif
}
#ifdef __EMSCRIPTEN__
#include "emscripten.h"

static void LoopCallback(void* arg)
{
    static_cast<dae::Minigin*>(arg)->RunOneFrame();
}
#endif
// Why bother with this? Because sometimes students have a different SDL version installed on their pc.
// That is not a problem unless for some reason the dll's from this project are not copied next to the exe.
// These entries in the debug output help to identify that issue.
static void PrintSDLVersion()
{
    SDL_version version{};
    SDL_VERSION(&version);
    LogSDLVersion("We compiled against SDL version ", version);

    SDL_GetVersion(&version);
    LogSDLVersion("We linked against SDL version ", version);

    SDL_IMAGE_VERSION(&version);
    LogSDLVersion("We compiled against SDL_image version ", version);

    version = *IMG_Linked_Version();
    LogSDLVersion("We linked against SDL_image version ", version);

    SDL_TTF_VERSION(&version)
    LogSDLVersion("We compiled against SDL_ttf version ", version);

    version = *TTF_Linked_Version();
    LogSDLVersion("We linked against SDL_ttf version ", version);
}

dae::Minigin::Minigin(const std::filesystem::path& dataPath)
{
    PrintSDLVersion();

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        throw std::runtime_error(std::string("SDL_Init Error: ") +
                                 SDL_GetError());
    }

    m_window = SDL_CreateWindow("Programming 4 assignment",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                m_WindowWidth,
                                m_WindowHeight,
                                SDL_WINDOW_OPENGL);
    if (m_window == nullptr)
    {
        throw std::runtime_error(std::string("SDL_CreateWindow Error: ") +
                                 SDL_GetError());
    }

    Renderer::GetInstance().Init(m_window);
    ResourceManager::GetInstance().Init(dataPath);
    ServiceLocator::GetInstance().RegisterSoundSystem(
        std::make_unique<SDLSoundSystem>(dataPath));
}

dae::Minigin::~Minigin()
{
    Renderer::GetInstance().Destroy();
    SDL_DestroyWindow(m_window);
    m_window = nullptr;
    SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
    load();
#ifndef __EMSCRIPTEN__
    GameLoop();
#else
    emscripten_set_main_loop_arg(&LoopCallback, this, 0, true);
#endif
}

void dae::Minigin::GameLoop()
{
    using namespace std::chrono;
    using clock = high_resolution_clock;
    while (m_continue)
    {
        auto& engineTime = EngineTime::GetInstance();
        auto& sceneManager = SceneManager::GetInstance();

        engineTime.Update();
        m_lag += engineTime.GetDeltaTime();

        m_continue = InputManager::GetInstance().ProcessInput();

        sceneManager.Update();
        while (m_lag >= engineTime.GetFixedTimeStep())
        {
            sceneManager.FixedUpdate();
            CollisionSystem::GetInstance().ProcessCollisions();
            m_lag -= engineTime.GetFixedTimeStep();
        }
        sceneManager.LateUpdate();
        Renderer::GetInstance().Render();

        const auto sleepTime = std::chrono::duration<float>(
            engineTime.GetLastTime() + milliseconds(m_msPerFrame) -
            high_resolution_clock::now());
        std::this_thread::sleep_for(sleepTime);
    }
}
