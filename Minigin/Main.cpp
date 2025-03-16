#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <steam_api.h>

#include "Minigin.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Scene.h"

#include "ComponentsHeader.h"

#include <filesystem>
#include <memory>
namespace fs = std::filesystem;

static constexpr glm::vec2 FPS_POS = { 10.0f, 6.0f };
static constexpr glm::vec2 LIVES_UI_POS = { 10.0f, 100.0f };
static constexpr glm::vec2 SCORE_UI_POS = { 10.0f, 110.0f };
static constexpr glm::vec2 LOGO_POS = { 216.0f, 180.0f };
static constexpr glm::vec2 TEXT_POS = { 80.0f, 20.0f };
static constexpr float OFFSET = 20.0f;
static constexpr float RADIUS = 40.0f;
static constexpr float INSTRUCTION_OFFSET_1 = 60.0f;
static constexpr float INSTRUCTION_OFFSET_2 = 80.0f;
static constexpr float DIVISOR = 2.0f;
static constexpr int BASE_SPEED = 100;
static constexpr int START_LIVES = 3;
static constexpr int SCORE_TEN = 10;
static constexpr int LIVES_UI_OFFSET_Y = 25;
static constexpr int SCORE_UI_OFFSET_Y = 25;
static constexpr uint8_t LARGE_FONT_SIZE = 36;
static constexpr uint8_t SMALL_FONT_SIZE = 14;

static void SetupPlayers(const int windowWidth,
                         const int windowHeight,
                         dae::RenderComponent*& RenderComp,
                         dae::Scene& scene,
                         dae::Font& smallFont)
{
    for (int i = 0; i < 2; ++i)
    {
        glm::vec2 middlePos = { windowWidth / 2, windowHeight / 2 };
        glm::vec2 playerPos = {
            middlePos[0] + (OFFSET * static_cast<float>(i - 1)), middlePos[1]
        };
        auto player =
            std::make_unique<dae::GameObject>("player" + std::to_string(i));
        RenderComp = player->AddComponent<dae::RenderComponent>();
        RenderComp->SetTexture(i == 0 ? "ChefPeterPepperF.png" :
                                        "ChefPeterPepperB.png");
        player->GetComponent<dae::TransformComponent>()->SetWorldPosition(
            playerPos);

        auto* livesComp =
            player->AddComponent<dae::LivesComponent>(START_LIVES);
        auto* scoreComp = player->AddComponent<dae::ScoreComponent>();

        auto* inputComp = player->AddComponent<dae::PlayerInputComponent>(
            static_cast<uint8_t>(i));
        inputComp->SetSpeed(BASE_SPEED * (i + 1));
        scene.Add(std::move(player));

        // create UI elements
        auto livesUI =
            std::make_unique<dae::GameObject>("LivesUI" + std::to_string(i));
        livesUI->AddComponent<dae::LivesUIComponent>(smallFont, livesComp);
        livesUI->GetComponent<dae::TransformComponent>()->SetWorldPosition(
            LIVES_UI_POS + glm::vec2(0, i * LIVES_UI_OFFSET_Y));
        scene.Add(std::move(livesUI));

        auto scoreUI =
            std::make_unique<dae::GameObject>("scoreUI" + std::to_string(i));
        scoreUI->AddComponent<dae::ScoreUIComponent>(smallFont, scoreComp);
        scoreUI->GetComponent<dae::TransformComponent>()->SetWorldPosition(
            SCORE_UI_POS + glm::vec2(0, i * SCORE_UI_OFFSET_Y));
        scene.Add(std::move(scoreUI));

        auto Achievements =
            std::make_unique<dae::GameObject>("Achiements" + std::to_string(i));
        Achievements->AddComponent<dae::AchievementComponent>(scoreComp);
        scene.Add(std::move(Achievements));
    }
}

static void load(const int windowWidth, const int windowHeight)
{
    auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

    auto go = std::make_unique<dae::GameObject>("background");
    auto* RenderComp = go->AddComponent<dae::RenderComponent>();
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

    auto& smallFont = dae::ResourceManager::GetInstance().LoadFont(
        "Lingua.otf", SMALL_FONT_SIZE);
    go = std::make_unique<dae::GameObject>("fps");
    go->AddComponent<dae::FPSComponent>(smallFont);
    go->GetComponent<dae::TransformComponent>()->SetWorldPosition(FPS_POS);
    scene.Add(std::move(go));

    //'use the D-Pad (on the first controller) or the arrow keys to move the frontfacing player'
    go = std::make_unique<dae::GameObject>("instruction0");
    go->AddComponent<dae::TextComponent>(
        "Use the D-Pad to move the frontfacing player, x to inflict damage, A "
        "and B to pick up pellets",
        smallFont);
    go->GetComponent<dae::TransformComponent>()->SetWorldPosition(
        { FPS_POS[0], FPS_POS[1] + INSTRUCTION_OFFSET_1 });
    scene.Add(std::move(go));

    //'use the D-Pad (on the second controller) or the WASD keys to move the backfacing player'
    go = std::make_unique<dae::GameObject>("instruction1");
    go->AddComponent<dae::TextComponent>(
        "Use the D-Pad to move the backfacing player, C to inflict damage, Z "
        "and X to pick up pellets",
        smallFont);
    go->GetComponent<dae::TransformComponent>()->SetWorldPosition(
        { FPS_POS[0], FPS_POS[1] + INSTRUCTION_OFFSET_2 });
    scene.Add(std::move(go));

    SetupPlayers(windowWidth, windowHeight, RenderComp, scene, smallFont);
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
    if (!SteamAPI_Init())
    {
        std::cerr << "Fatal Error - Steam must be running to play this "
                     "game (SteamAPI_Init() failed)."
                  << std::endl;
        return 1;
    }
    else
    {
        std::cout << "Successfully initialized steam." << std::endl;
        //
    }

    dae::Minigin engine(data_location);
    engine.Run([&engine]()
               { load(engine.GetWindowWidth(), engine.GetWindowHeight()); });
    SteamAPI_Shutdown();
    return 0;
}
