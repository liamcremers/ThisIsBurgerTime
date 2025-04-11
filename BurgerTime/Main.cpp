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
#include "BurgerTimeLayers.h"
#include "CollisionSystem.h"

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
static constexpr int G_SIZE = 16;
static constexpr float GRAVITY = 9.81f;
static constexpr glm::vec2 FLOOR_SIZE = { G_SIZE, G_SIZE / 4.f };
static constexpr glm::vec2 FLOOR_OFFSET = { 0, 10 };

static void SetupPlayers(const int windowWidth, const int windowHeight)
{
    auto& scene = dae::SceneManager::GetInstance().CreateScene("PlayerScene");
    auto& smallFont = dae::ResourceManager::GetInstance().LoadFont(
        "Lingua.otf", SMALL_FONT_SIZE);
    for (int i = 0; i < 2; ++i)
    {
        glm::vec2 middlePos = { windowWidth / 2, windowHeight / 2 + 20 };
        glm::vec2 playerPos = {
            middlePos[0] + (OFFSET * static_cast<float>(i - 1)), middlePos[1]
        };
        auto player =
            std::make_unique<dae::GameObject>("player" + std::to_string(i));
        auto* renderComp = player->AddComponent<dae::RenderComponent>();
        renderComp->SetTexture(i == 0 ? "ChefPeterPepperF.png" :
                                        "ChefPeterPepperB.png");
        renderComp->Scale(2);
        player->SetLocalPosition(playerPos);

        auto* physics = player->AddComponent<dae::PhysicsComponent>();
        physics->SetGravity(GRAVITY);

        auto* collider = player->AddComponent<dae::ColliderComponent>(
            glm::vec2{ 2 * G_SIZE, 2 * G_SIZE });
        collider->SetCollisionLayer(
            static_cast<uint16_t>(BurgerTime::CollisionLayer::Player));
        collider->SetCollisionMask(BurgerTime::PLAYER_COLLISION_MASK);
        dae::CollisionSystem::GetInstance().RegisterCollider(collider);

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
        livesUIgo->SetLocalPosition(LIVES_UI_POS +
                                    glm::vec2(0, i * LIVES_UI_OFFSET_Y));
        scene.Add(std::move(livesUIgo));

        auto scoreUIgo =
            std::make_unique<dae::GameObject>("scoreUI" + std::to_string(i));
        scoreUIgo->AddComponent<dae::ScoreUIComponent>(smallFont, scoreComp);
        scoreUIgo->SetLocalPosition(SCORE_UI_POS +
                                    glm::vec2(0, i * SCORE_UI_OFFSET_Y));
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
    fps->SetLocalPosition(FPS_POS);
    FPSScene.Add(std::move(fps));
}

static void CreateFloorPlane(dae::Scene& scene,
                             const glm::vec2& startPos,
                             std::initializer_list<int> segments,
                             const std::string& baseName = "Floor")
{
    static bool hasScaledTexture = false;
    int tileCount = 0;
    int positionOffset = 0;

    for (int segment : segments)
    {
        if (segment > 0)
        {
            for (int i = 0; i < segment; ++i)
            {
                auto go = std::make_unique<dae::GameObject>(
                    baseName + std::to_string(tileCount++));
                auto* rendererComp = go->AddComponent<dae::RenderComponent>();
                rendererComp->SetTexture("Floor.png");
                auto* colliderComp = go->AddComponent<dae::ColliderComponent>();
                colliderComp->SetSize(FLOOR_SIZE);
                colliderComp->SetOffset(FLOOR_OFFSET);
                colliderComp->SetCollisionLayer(
                    static_cast<uint16_t>(BurgerTime::CollisionLayer::Floor));
                colliderComp->SetCollisionMask(
                    BurgerTime::FLOOR_COLLISION_MASK);
                go->SetLocalPosition(
                    startPos + glm::vec2((positionOffset + i) * G_SIZE, 0));
                dae::CollisionSystem::GetInstance().RegisterCollider(
                    colliderComp);

                if (!hasScaledTexture)
                {
                    rendererComp->Scale(2);
                    hasScaledTexture = true;
                }

                scene.Add(std::move(go));
            }
            positionOffset += segment;
        }
        else if (segment < 0)
        {
            positionOffset += -segment;
        }
    }
}

static void CreateStairs(dae::Scene& scene,
                         const glm::vec2& startPos,
                         int nrStairs,
                         bool bGreen,
                         const std::string& baseName = "Stair")
{
    // Track which textures have been scaled to avoid scaling the same texture multiple times
    static bool hasScaledTextureGL = false;
    static bool hasScaledTextureGR = false;
    static bool hasScaledTextureBL = false;
    static bool hasScaledTextureBR = false;

    for (int i = 0; i < nrStairs; ++i)
    {
        // Create left stair piece
        auto goL = std::make_unique<dae::GameObject>(baseName + "L" +
                                                     std::to_string(i));
        auto* rendererCompL = goL->AddComponent<dae::RenderComponent>();

        if (bGreen)
        {
            rendererCompL->SetTexture("Stair_G_L.png");
            if (!hasScaledTextureGL)
            {
                rendererCompL->Scale(2);
                hasScaledTextureGL = true;
            }
        }
        else
        {
            rendererCompL->SetTexture("Stair_B_L.png");
            if (!hasScaledTextureBL)
            {
                rendererCompL->Scale(2);
                hasScaledTextureBL = true;
            }
        }

        goL->SetLocalPosition(startPos + glm::vec2(0, i * G_SIZE));

        // Create right stair piece
        auto goR = std::make_unique<dae::GameObject>(baseName + "R" +
                                                     std::to_string(i));
        auto* rendererCompR = goR->AddComponent<dae::RenderComponent>();

        if (bGreen)
        {
            rendererCompR->SetTexture("Stair_G_R.png");
            if (!hasScaledTextureGR)
            {
                rendererCompR->Scale(2);
                hasScaledTextureGR = true;
            }
        }
        else
        {
            rendererCompR->SetTexture("Stair_B_R.png");
            if (!hasScaledTextureBR)
            {
                rendererCompR->Scale(2);
                hasScaledTextureBR = true;
            }
        }

        // Position the right stair piece next to the left piece
        goR->SetLocalPosition(startPos + glm::vec2(G_SIZE, i * G_SIZE));

        scene.Add(std::move(goL));
        scene.Add(std::move(goR));
    }
}

static void CreateBurgerPlateStack(dae::Scene& scene,
                                   const glm::vec2& startPos,
                                   const std::string& baseName = "BurgerPlate")
{
    static bool hasScaledTextureTL = false;
    static bool hasScaledTextureML = false;
    static bool hasScaledTextureM = false;
    static bool hasScaledTextureMR = false;
    static bool hasScaledTextureTR = false;

    // Top Left Plate
    auto goTL = std::make_unique<dae::GameObject>(baseName + "_TL");
    auto* rendererCompTL = goTL->AddComponent<dae::RenderComponent>();
    rendererCompTL->SetTexture("BurgerPlate_T_L.png");
    if (!hasScaledTextureTL)
    {
        rendererCompTL->Scale(2);
        hasScaledTextureTL = true;
    }
    goTL->SetLocalPosition(startPos);
    scene.Add(std::move(goTL));

    // Middle Left Plate
    auto goML = std::make_unique<dae::GameObject>(baseName + "_ML");
    auto* rendererCompML = goML->AddComponent<dae::RenderComponent>();
    rendererCompML->SetTexture("BurgerPlate_M_L.png");
    if (!hasScaledTextureML)
    {
        rendererCompML->Scale(2);
        hasScaledTextureML = true;
    }
    goML->SetLocalPosition(startPos + glm::vec2(0, G_SIZE));
    scene.Add(std::move(goML));

    // Middle Plates (4x)
    for (int i = 0; i < 4; ++i)
    {
        auto goM = std::make_unique<dae::GameObject>(baseName + "_M" +
                                                     std::to_string(i));
        auto* rendererCompM = goM->AddComponent<dae::RenderComponent>();
        rendererCompM->SetTexture("BurgerPlate_M.png");
        if (!hasScaledTextureM)
        {
            rendererCompM->Scale(2);
            hasScaledTextureM = true;
        }
        goM->SetLocalPosition(startPos + glm::vec2((i + 1) * G_SIZE, G_SIZE));
        scene.Add(std::move(goM));
    }

    static constexpr int PLATE_LENGTH = 5;
    // Middle Right Plate
    auto goMR = std::make_unique<dae::GameObject>(baseName + "_MR");
    auto* rendererCompMR = goMR->AddComponent<dae::RenderComponent>();
    rendererCompMR->SetTexture("BurgerPlate_M_R.png");
    if (!hasScaledTextureMR)
    {
        rendererCompMR->Scale(2);
        hasScaledTextureMR = true;
    }
    goMR->SetLocalPosition(startPos + glm::vec2(PLATE_LENGTH * G_SIZE, G_SIZE));
    scene.Add(std::move(goMR));

    // Top Right Plate
    auto goTR = std::make_unique<dae::GameObject>(baseName + "_TR");
    auto* rendererCompTR = goTR->AddComponent<dae::RenderComponent>();
    rendererCompTR->SetTexture("BurgerPlate_T_R.png");
    if (!hasScaledTextureTR)
    {
        rendererCompTR->Scale(2);
        hasScaledTextureTR = true;
    }
    goTR->SetLocalPosition(startPos + glm::vec2(PLATE_LENGTH * G_SIZE, 0));
    scene.Add(std::move(goTR));
}

static void SetupLevel0()
{
    static constexpr glm::vec2 COUNTER_POS = { G_SIZE * 2, G_SIZE * 6 };

    auto& Level0Scene = dae::SceneManager::GetInstance().CreateScene("Level0");

    auto go = std::make_unique<dae::GameObject>("LevelCounter");
    auto* renderComp = go->AddComponent<dae::RenderComponent>();
    renderComp->SetTexture("LevelCounter.png");
    renderComp->Scale(2);
    go->SetLocalPosition(COUNTER_POS);
    Level0Scene.Add(std::move(go));
#pragma region FLOORS
    static constexpr glm::vec2 TOP_FLOOR_POS = { G_SIZE * 3, G_SIZE * 5 };
    static constexpr auto TOP_FLOOR_SEGMENTS = { 26 };
    static constexpr glm::vec2 MID_FLOOR_1_POS = { G_SIZE * 3, G_SIZE * 9 };
    static constexpr auto MID_FLOOR_1_SEGMENTS = { 8, -4, 14 };
    static constexpr glm::vec2 MID_FLOOR_2_POS = { G_SIZE * 3, G_SIZE * 11 };
    static constexpr auto MID_FLOOR_2_SEGMENTS = { -6, 8 };
    static constexpr glm::vec2 MID_FLOOR_3_POS = { G_SIZE * 3, G_SIZE * 13 };
    static constexpr auto MID_FLOOR_3_SEGMENTS = { 8, -10, 8 };
    static constexpr glm::vec2 MID_FLOOR_4_POS = { G_SIZE * 3, G_SIZE * 15 };
    static constexpr auto MID_FLOOR_4_SEGMENTS = { -6, 14 };
    static constexpr glm::vec2 MID_FLOOR_5_POS = { G_SIZE * 3, G_SIZE * 17 };
    static constexpr auto MID_FLOOR_5_SEGMENTS = { -18, 8 };
    static constexpr glm::vec2 MID_FLOOR_6_POS = { G_SIZE * 3, G_SIZE * 19 };
    static constexpr auto MID_FLOOR_6_SEGMENTS = { 20 };
    static constexpr glm::vec2 BOTTOM_FLOOR_POS = { G_SIZE * 3, G_SIZE * 23 };
    static constexpr auto BOTTOM_FLOOR_SEGMENTS = { 26 };

    CreateFloorPlane(
        Level0Scene, TOP_FLOOR_POS, TOP_FLOOR_SEGMENTS, "TopFloor");

    CreateFloorPlane(
        Level0Scene, MID_FLOOR_1_POS, MID_FLOOR_1_SEGMENTS, "MidFloor1");
    CreateFloorPlane(
        Level0Scene, MID_FLOOR_2_POS, MID_FLOOR_2_SEGMENTS, "MidFloor2");
    CreateFloorPlane(
        Level0Scene, MID_FLOOR_3_POS, MID_FLOOR_3_SEGMENTS, "MidFloor3");
    CreateFloorPlane(
        Level0Scene, MID_FLOOR_4_POS, MID_FLOOR_4_SEGMENTS, "MidFloor4");
    CreateFloorPlane(
        Level0Scene, MID_FLOOR_5_POS, MID_FLOOR_5_SEGMENTS, "MidFloor5");
    CreateFloorPlane(
        Level0Scene, MID_FLOOR_6_POS, MID_FLOOR_6_SEGMENTS, "MidFloor6");

    CreateFloorPlane(
        Level0Scene, BOTTOM_FLOOR_POS, BOTTOM_FLOOR_SEGMENTS, "BottomFloor");

#pragma endregion
#pragma region STAIRS
    static constexpr glm::vec2 STAIR_1_POS{ G_SIZE * 3, G_SIZE * 6 };
    static constexpr int STAIR_1_COUNT{ 4 };
    static constexpr bool STAIR_1_GREEN{ true };
    static constexpr glm::vec2 STAIR_2_POS{ G_SIZE * 3, G_SIZE * 14 };
    static constexpr int STAIRS_2_COUNT{ 10 };
    static constexpr bool STAIR_2_GREEN{ true };
    static constexpr glm::vec2 STAIR_3_POS{ G_SIZE * 6, G_SIZE * 10 };
    static constexpr int STAIR_3_COUNT{ 10 };
    static constexpr bool STAIR_3_GREEN{ false };
    static constexpr glm::vec2 STAIR_4_POS{ G_SIZE * 9, G_SIZE * 6 };
    static constexpr int STAIR_4_COUNT{ 18 };
    static constexpr bool STAIR_4_GREEN{ true };
    static constexpr glm::vec2 STAIR_5_POS{ G_SIZE * 12, G_SIZE * 6 };
    static constexpr int STAIR_5_COUNT{ 6 };
    static constexpr bool STAIR_5_GREEN{ false };
    static constexpr glm::vec2 STAIR_6_POS{ G_SIZE * 15, G_SIZE * 6 };
    static constexpr int STAIR_6_COUNT{ 18 };
    static constexpr bool STAIR_6_GREEN{ true };
    static constexpr glm::vec2 STAIR_7_POS{ G_SIZE * 18, G_SIZE * 10 };
    static constexpr int STAIR_7_COUNT{ 6 };
    static constexpr bool STAIR_7_GREEN{ false };
    static constexpr glm::vec2 STAIR_8_POS{ G_SIZE * 21, G_SIZE * 6 };
    static constexpr int STAIR_8_COUNT{ 18 };
    static constexpr bool STAIR_8_GREEN{ true };
    static constexpr glm::vec2 STAIR_9_POS{ G_SIZE * 24, G_SIZE * 14 };
    static constexpr int STAIR_9_COUNT{ 10 };
    static constexpr bool STAIR_9_GREEN{ false };
    static constexpr glm::vec2 STAIR_10_POS{ G_SIZE * 27, G_SIZE * 6 };
    static constexpr int STAIR_10_COUNT{ 8 };
    static constexpr bool STAIR_10_GREEN{ true };
    static constexpr glm::vec2 STAIR_11_POS{ G_SIZE * 27, G_SIZE * 18 };
    static constexpr int STAIR_11_COUNT{ 6 };
    static constexpr bool STAIR_11_GREEN{ true };

    CreateStairs(Level0Scene, STAIR_3_POS, STAIR_3_COUNT, STAIR_3_GREEN);
    CreateStairs(Level0Scene, STAIR_1_POS, STAIR_1_COUNT, STAIR_1_GREEN);
    CreateStairs(Level0Scene, STAIR_2_POS, STAIRS_2_COUNT, STAIR_2_GREEN);
    CreateStairs(Level0Scene, STAIR_4_POS, STAIR_4_COUNT, STAIR_4_GREEN);
    CreateStairs(Level0Scene, STAIR_5_POS, STAIR_5_COUNT, STAIR_5_GREEN);
    CreateStairs(Level0Scene, STAIR_6_POS, STAIR_6_COUNT, STAIR_6_GREEN);
    CreateStairs(Level0Scene, STAIR_7_POS, STAIR_7_COUNT, STAIR_7_GREEN);
    CreateStairs(Level0Scene, STAIR_8_POS, STAIR_8_COUNT, STAIR_8_GREEN);
    CreateStairs(Level0Scene, STAIR_9_POS, STAIR_9_COUNT, STAIR_9_GREEN);
    CreateStairs(Level0Scene, STAIR_10_POS, STAIR_10_COUNT, STAIR_10_GREEN);
    CreateStairs(Level0Scene, STAIR_11_POS, STAIR_11_COUNT, STAIR_11_GREEN);
#pragma endregion
#pragma region BURGERPLATE
    static constexpr glm::vec2 BURGERPLATE_POS_1 = { G_SIZE * 4, G_SIZE * 27 };
    static constexpr glm::vec2 BURGERPLATE_POS_2 = { G_SIZE * 10, G_SIZE * 27 };
    static constexpr glm::vec2 BURGERPLATE_POS_3 = { G_SIZE * 16, G_SIZE * 27 };
    static constexpr glm::vec2 BURGERPLATE_POS_4 = { G_SIZE * 22, G_SIZE * 27 };

    CreateBurgerPlateStack(Level0Scene, BURGERPLATE_POS_1);
    CreateBurgerPlateStack(Level0Scene, BURGERPLATE_POS_2);
    CreateBurgerPlateStack(Level0Scene, BURGERPLATE_POS_3);
    CreateBurgerPlateStack(Level0Scene, BURGERPLATE_POS_4);
#pragma endregion
}

static void load(const int windowWidth, const int windowHeight)
{
    SetupLevel0();
    SetupPlayers(windowWidth, windowHeight);
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
