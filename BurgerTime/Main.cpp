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

static constexpr glm::vec2 FPS_POS = { 5.0f, 6.0f };
static constexpr glm::vec2 INSTRUCTION_POS = { 58.0f, 6.0f };
static constexpr glm::vec2 LIVES_UI_POS = { 5.0f, 20.0f };
static constexpr glm::vec2 SCORE_UI_POS = { 5.0f, 30.0f };
static constexpr glm::vec2 LOGO_POS = { 216.0f, 180.0f };
static constexpr glm::vec2 TEXT_POS = { 80.0f, 20.0f };
static constexpr float OFFSET = 20.0f;
static constexpr float RADIUS = 40.0f;
static constexpr float INSTRUCTION_OFFSET_1 = 0.0f;
static constexpr float INSTRUCTION_OFFSET_2 = 20.0f;
static constexpr float DIVISOR = 2.0f;
static constexpr int BASE_SPEED = 100;
static constexpr int START_LIVES = 3;
static constexpr int SCORE_TEN = 10;
static constexpr int LIVES_UI_OFFSET_Y = 25;
static constexpr int SCORE_UI_OFFSET_Y = 25;
static constexpr uint8_t LARGE_FONT_SIZE = 36;
static constexpr uint8_t SMALL_FONT_SIZE = 14;
static constexpr uint8_t INSTRUCTION_FONT_SIZE = 11;
static constexpr auto INSTRUCTION_TXT_0 =
    "Use the D-Pad to move the frontfacing player, x to inflict damage, A and "
    "B to pick up pellets";
static constexpr auto INSTRUCTION_TXT_1 =
    "Use the D-Pad to move the backfacing player, C to inflict damage, Z "
    "and Q to pick up pellets";

static void SetupPlayers(const int windowWidth, const int windowHeight)
{
    auto& scene = dae::SceneManager::GetInstance().CreateScene("PlayerScene");
    auto& smallFont = dae::ResourceManager::GetInstance().LoadFont(
        "Lingua.otf", SMALL_FONT_SIZE);
    for (int i = 0; i < 2; ++i)
    {
        glm::vec2 middlePos = { windowWidth / 2, windowHeight / 2 };
        glm::vec2 playerPos = {
            middlePos[0] + (OFFSET * static_cast<float>(i - 1)), middlePos[1]
        };
        auto player =
            std::make_unique<dae::GameObject>("player" + std::to_string(i));
        auto* renderComp = player->AddComponent<dae::RenderComponent>();
        renderComp->SetTexture(i == 0 ? "ChefPeterPepperF.png" :
                                        "ChefPeterPepperB.png");
        renderComp->Scale(2);
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
        auto livesUIgo =
            std::make_unique<dae::GameObject>("LivesUI" + std::to_string(i));
        livesUIgo->AddComponent<dae::LivesUIComponent>(smallFont, livesComp);
        livesUIgo->GetComponent<dae::TransformComponent>()->SetWorldPosition(
            LIVES_UI_POS + glm::vec2(0, i * LIVES_UI_OFFSET_Y));
        scene.Add(std::move(livesUIgo));

        auto scoreUIgo =
            std::make_unique<dae::GameObject>("scoreUI" + std::to_string(i));
        scoreUIgo->AddComponent<dae::ScoreUIComponent>(smallFont, scoreComp);
        scoreUIgo->GetComponent<dae::TransformComponent>()->SetWorldPosition(
            SCORE_UI_POS + glm::vec2(0, i * SCORE_UI_OFFSET_Y));
        scene.Add(std::move(scoreUIgo));
    }
}

static void SetupFPSScene()
{
    auto& FPSScene = dae::SceneManager::GetInstance().CreateScene("FPS");
    auto& fpsFont = dae::ResourceManager::GetInstance().LoadFont(
        "Lingua.otf", SMALL_FONT_SIZE);
    auto fps = std::make_unique<dae::GameObject>("fps");
    fps->AddComponent<dae::FPSComponent>(fpsFont);
    fps->GetComponent<dae::TransformComponent>()->SetWorldPosition(FPS_POS);
    FPSScene.Add(std::move(fps));
}

static void SetupInstructions()
{
    auto& scene = dae::SceneManager::GetInstance().CreateScene("Instructions");
    auto& instructionFont = dae::ResourceManager::GetInstance().LoadFont(
        "Lingua.otf", INSTRUCTION_FONT_SIZE);
    auto go = std::make_unique<dae::GameObject>("instruction0");
    go->AddComponent<dae::TextComponent>(INSTRUCTION_TXT_0, instructionFont);
    go->GetComponent<dae::TransformComponent>()->SetWorldPosition(
        { INSTRUCTION_POS[0], INSTRUCTION_POS[1] + INSTRUCTION_OFFSET_1 });
    scene.Add(std::move(go));

    go = std::make_unique<dae::GameObject>("instruction1");
    go->AddComponent<dae::TextComponent>(INSTRUCTION_TXT_1, instructionFont);
    go->GetComponent<dae::TransformComponent>()->SetWorldPosition(
        { INSTRUCTION_POS[0], INSTRUCTION_POS[1] + INSTRUCTION_OFFSET_2 });
    scene.Add(std::move(go));
}

static void SetupLevel0()
{
    auto& Level0Scene = dae::SceneManager::GetInstance().CreateScene("Level0");

    auto go = std::make_unique<dae::GameObject>("levelBackground");
    auto* rendererComp = go->AddComponent<dae::RenderComponent>();
    rendererComp->SetTexture("Level0.png");
    rendererComp->Scale(2);
    Level0Scene.Add(std::move(go));
}

static void load(const int windowWidth, const int windowHeight)
{
    SetupLevel0();
    SetupPlayers(windowWidth, windowHeight);
    SetupInstructions();
    SetupFPSScene();
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
