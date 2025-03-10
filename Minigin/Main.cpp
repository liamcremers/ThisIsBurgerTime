#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Scene.h"

#include "ComponentsHeader.h"

#include <filesystem>
#include <memory>
namespace fs = std::filesystem;

static constexpr glm::vec2 FPS_POS = { 10.0f, 6.0f };
static constexpr glm::vec2 LOGO_POS = { 216.0f, 180.0f };
static constexpr glm::vec2 TEXT_POS = { 80.0f, 20.0f };
static constexpr float OFFSET = 20.0f;
static constexpr float RADIUS = 40.0f;
static constexpr uint8_t LARGE_FONT_SIZE = 36;
static constexpr uint8_t SMALL_FONT_SIZE = 14;

static void load(const int windowWidth, const int windowHeight)
{
    auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

    auto go = std::make_unique<dae::GameObject>("background");
    auto RenderComp = go->AddComponent<dae::RenderComponent>();
    RenderComp->SetTexture("background.tga");
    scene.Add(std::move(go));

    go = std::make_unique<dae::GameObject>("logo");
    RenderComp = go->AddComponent<dae::RenderComponent>();
    RenderComp->SetTexture("logo.tga");
    go->GetComponent<dae::TransformComponent>()->SetWorldPosition(LOGO_POS);
    scene.Add(std::move(go));

    auto& font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf",
                                                              LARGE_FONT_SIZE);
    go = std::make_unique<dae::GameObject>("Title");
    go->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
    go->GetComponent<dae::TransformComponent>()->SetWorldPosition(TEXT_POS);
    scene.Add(std::move(go));

    auto& fpsFont = dae::ResourceManager::GetInstance().LoadFont(
        "Lingua.otf", SMALL_FONT_SIZE);
    go = std::make_unique<dae::GameObject>("fps");
    go->AddComponent<dae::FPSComponent>(fpsFont);
    go->GetComponent<dae::TransformComponent>()->SetWorldPosition({ FPS_POS });
    scene.Add(std::move(go));

    //'use the D-Pad (on the first controller) or the arrow keys to move the frontfacing player'
    go = std::make_unique<dae::GameObject>("instruction0");
    go.get()->AddComponent<dae::TextComponent>(
        "Use the D-Pad (on the first controller) or the arrow keys to move the "
        "frontfacing player",
        fpsFont);
    go.get()->GetComponent<dae::TransformComponent>()->SetWorldPosition(
        { FPS_POS.x, FPS_POS.y + 60.0f });
    scene.Add(std::move(go));

    //'use the D-Pad (on the second controller) or the WASD keys to move the backfacing player'
    go = std::make_unique<dae::GameObject>("instruction1");
    go.get()->AddComponent<dae::TextComponent>(
        "Use the D-Pad (on the second controller) or the WASD keys to move the "
        "backfacing player",
        fpsFont);
    go.get()->GetComponent<dae::TransformComponent>()->SetWorldPosition(
        { FPS_POS.x, FPS_POS.y + 80.0f });
    scene.Add(std::move(go));

    // Create 2 player objects
    for (int i = 0; i < 2; ++i)
    {
        glm::vec2 middlePos = { windowWidth / 2.f, windowHeight / 2.f };
        glm::vec2 playerPos = { middlePos.x + OFFSET * (i - 1), middlePos.y };
        go = std::make_unique<dae::GameObject>("player" + std::to_string(i));
        RenderComp = go->AddComponent<dae::RenderComponent>();
        RenderComp->SetTexture(i == 0 ? "ChefPeterPepperF.png" :
                                        "ChefPeterPepperB.png");
        go->GetComponent<dae::TransformComponent>()->SetWorldPosition(
            { playerPos });
        auto playerComp =
            go->AddComponent<dae::PlayerComponent>(static_cast<uint8_t>(i));
        playerComp->SetSpeed(100 * (i + 1));

        scene.Add(std::move(go));
    }
}

auto main(int, char*[]) -> int
{
#if __EMSCRIPTEN__
    fs::path data_location = "";
#else
    fs::path data_location = "./Data/";
    if (!fs::exists(data_location))
        data_location = "../Data/";
#endif
    dae::Minigin engine(data_location);
    engine.Run([&engine]()
               { load(engine.GetWindowWidth(), engine.GetWindowHeight()); });
    return 0;
}
