#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <ComponentsHeader.h>
#include <Minigin.h>
#include <ResourceManager.h>
#include <InputManager.h>
#include <CollisionSystem.h>
#include <Scene.h>
#include <ServiceLocator.h>
#include <SoundSystem.h>

#include "FPSComponent.h"
#include "BurgerTimeLayers.h"
#include "LevelGrid.h"
#include "BurgerPartComponent.h"
#include "BurgerTimeSoundIds.h"
#include "PlayerComponent.h"
#include "EnemyComponent.h"
#include "BurgerGroup.h"
#include "LivesUIComponent.h"
#include "ScoreUIComponent.h"
#include "LivesComponent.h"
#include "ScoreComponent.h"
#include "InputComponent.h"

#include <filesystem>
#include <memory>
namespace fs = std::filesystem;

static constexpr int WINDOW_WIDTH = dae::Minigin::GetWindowWidth();
static constexpr int WINDOW_HEIGHT = dae::Minigin::GetWindowHeight();
static constexpr glm::vec2 FPS_POS = { 5.0f, 6.0f };
static constexpr glm::vec2 LIVES_UI_POS = { 5.0f, 20.0f };
static constexpr glm::vec2 SCORE_UI_POS = { 5.0f, 30.0f };
static constexpr float OFFSET = 20.0f;
static constexpr int START_LIVES = 3;
static constexpr int LIVES_UI_OFFSET_Y = 25;
static constexpr int SCORE_UI_OFFSET_Y = 25;
static constexpr uint8_t SMALL_FONT_SIZE = 14;
static constexpr int G_SIZE = 16;
static constexpr glm::vec2 FLOOR_SIZE = { G_SIZE, G_SIZE / 4.f };
static constexpr glm::vec2 FLOOR_OFFSET = { 0, 0 };
static constexpr int TOP_BORDER = 10;
static constexpr glm::ivec2 GRID_OFFSET = { 16, 10 };
static constexpr glm::vec2 PLAYER_START_POS = { WINDOW_WIDTH / 2,
                                                WINDOW_HEIGHT / 2 + 106 };

static void SetupPlayers()
{
    auto& scene = dae::SceneManager::GetInstance().CreateScene("PlayerScene");
    auto& smallFont = dae::ResourceManager::GetInstance().LoadFont(
        "Lingua.otf", SMALL_FONT_SIZE);
    {
        glm::vec2 playerPos = { PLAYER_START_POS[0] - OFFSET,
                                PLAYER_START_POS[1] };
        auto player = std::make_unique<dae::GameObject>("player");
        player->SetLocalPosition(playerPos);

        auto* collider = player->AddComponent<dae::ColliderComponent>(
            glm::vec2{ 2 * G_SIZE, 2 * G_SIZE });
        collider->SetCollisionLayer(
            static_cast<uint16_t>(CollisionLayer::Player));
        collider->SetCollisionType(CollisionType::Trigger);
        collider->SetCollisionMask(PLAYER_COLLISION_MASK);

        auto* physics = player->AddComponent<dae::PhysicsComponent>();
        physics->SetUseGravity(false);

        auto* livesComp = player->AddComponent<LivesComponent>(START_LIVES);
        auto* scoreComp = player->AddComponent<ScoreComponent>();

        auto* pPlayerComp = player->AddComponent<PlayerComponent>();
        player->AddComponent<InputComponent>(uint16_t{}, pPlayerComp);
        //player->AddComponent<PlayerInputComponent>(uint8_t{});

        dae::CollisionSystem::GetInstance().RegisterCollider(collider);
        scene.Add(std::move(player));

        // create UI elements
        auto livesUIgo = std::make_unique<dae::GameObject>("LivesUI");
        livesUIgo->AddComponent<LivesUIComponent>(smallFont, livesComp);
        livesUIgo->SetLocalPosition(LIVES_UI_POS);
        scene.Add(std::move(livesUIgo));

        auto scoreUIgo = std::make_unique<dae::GameObject>("scoreUI");
        scoreUIgo->AddComponent<ScoreUIComponent>(smallFont, scoreComp);
        scoreUIgo->SetLocalPosition(SCORE_UI_POS);
        scene.Add(std::move(scoreUIgo));
    }
    {
        glm::vec2 enemyPos = { PLAYER_START_POS[0], PLAYER_START_POS[1] };
        auto enemy = std::make_unique<dae::GameObject>("enemyPlayer");
        enemy->SetLocalPosition(enemyPos);

        auto* collider = enemy->AddComponent<dae::ColliderComponent>(
            glm::vec2{ 2 * G_SIZE, 2 * G_SIZE });
        collider->SetCollisionLayer(
            static_cast<uint16_t>(CollisionLayer::Enemy));
        collider->SetCollisionType(CollisionType::Trigger);
        collider->SetCollisionMask(ENEMY_COLLISION_MASK);

        auto* physics = enemy->AddComponent<dae::PhysicsComponent>();
        physics->SetUseGravity(false);

        auto* livesComp = enemy->AddComponent<LivesComponent>(START_LIVES);

        auto* pEnemyComp = enemy->AddComponent<EnemyComponent>();
        enemy->AddComponent<InputComponent>(uint8_t{ 1 }, pEnemyComp);

        dae::CollisionSystem::GetInstance().RegisterCollider(collider);
        scene.Add(std::move(enemy));

        // create UI elements
        auto livesUIgo = std::make_unique<dae::GameObject>("LivesUIEnemy");
        livesUIgo->AddComponent<LivesUIComponent>(smallFont, livesComp);
        livesUIgo->SetLocalPosition(LIVES_UI_POS +
                                    glm::vec2(0, LIVES_UI_OFFSET_Y));
        scene.Add(std::move(livesUIgo));
    }
}

static void SetupFPSScene()
{
    auto& FPSScene = dae::SceneManager::GetInstance().CreateScene("FPS");
    auto& fpsFont = dae::ResourceManager::GetInstance().LoadFont(
        "Lingua.otf", SMALL_FONT_SIZE);
    auto fps = std::make_unique<dae::GameObject>("fps");
    fps->AddComponent<FPSComponent>(fpsFont);
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
                    static_cast<uint16_t>(CollisionLayer::Floor));
                colliderComp->SetCollisionMask(BURGER_PART_COLLISION_MASK);
                colliderComp->SetCollisionType(CollisionType::Trigger);
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

static void CreateLadder(dae::Scene& scene,
                         const glm::vec2& startPos,
                         int nrLadders,
                         bool bGreen,
                         const std::string& baseName = "Ladder")
{
    // Track which textures have been scaled to avoid scaling the same texture multiple times
    static bool hasScaledTextureGL = false;
    static bool hasScaledTextureGR = false;
    static bool hasScaledTextureBL = false;
    static bool hasScaledTextureBR = false;

    for (int i = 0; i < nrLadders; ++i)
    {
        // Create left ladder piece
        auto goL = std::make_unique<dae::GameObject>(baseName + "L" +
                                                     std::to_string(i));
        auto* rendererCompL = goL->AddComponent<dae::RenderComponent>();

        if (bGreen)
        {
            rendererCompL->SetTexture("Ladder_G_L.png");
            if (!hasScaledTextureGL)
            {
                rendererCompL->Scale(2);
                hasScaledTextureGL = true;
            }
        }
        else
        {
            rendererCompL->SetTexture("Ladder_B_L.png");
            if (!hasScaledTextureBL)
            {
                rendererCompL->Scale(2);
                hasScaledTextureBL = true;
            }
        }

        goL->SetLocalPosition(startPos + glm::vec2(0, i * G_SIZE));

        // Create right ladder piece
        auto goR = std::make_unique<dae::GameObject>(baseName + "R" +
                                                     std::to_string(i));
        auto* rendererCompR = goR->AddComponent<dae::RenderComponent>();

        if (bGreen)
        {
            rendererCompR->SetTexture("Ladder_G_R.png");
            if (!hasScaledTextureGR)
            {
                rendererCompR->Scale(2);
                hasScaledTextureGR = true;
            }
        }
        else
        {
            rendererCompR->SetTexture("Ladder_B_R.png");
            if (!hasScaledTextureBR)
            {
                rendererCompR->Scale(2);
                hasScaledTextureBR = true;
            }
        }

        // Position the right ladder piece next to the left piece
        goR->SetLocalPosition(startPos + glm::vec2(G_SIZE, i * G_SIZE));

        auto* colliderL = goL->AddComponent<dae::ColliderComponent>(
            glm::vec2{ G_SIZE, G_SIZE });
        colliderL->SetCollisionLayer(
            static_cast<uint16_t>(CollisionLayer::Ladder));
        colliderL->SetCollisionMask(LADDER_COLLISION_MASK);
        colliderL->SetCollisionType(CollisionType::Trigger);
        dae::CollisionSystem::GetInstance().RegisterCollider(colliderL);

        auto* colliderR = goR->AddComponent<dae::ColliderComponent>(
            glm::vec2{ G_SIZE, G_SIZE });
        colliderR->SetCollisionLayer(
            static_cast<uint16_t>(CollisionLayer::Ladder));
        colliderR->SetCollisionMask(LADDER_COLLISION_MASK);
        colliderR->SetCollisionType(CollisionType::Trigger);
        dae::CollisionSystem::GetInstance().RegisterCollider(colliderR);

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

    // Top Left BurgerPlate
    auto goTL = std::make_unique<dae::GameObject>(baseName + "_TL");
    auto* rendererCompTL = goTL->AddComponent<dae::RenderComponent>();
    rendererCompTL->SetTexture("BurgerPlate_T_L.png");
    if (!hasScaledTextureTL)
    {
        rendererCompTL->Scale(2);
        hasScaledTextureTL = true;
    }
    auto* col =
        goTL->AddComponent<dae::ColliderComponent>(glm::vec2{ G_SIZE, G_SIZE });
    col->SetCollisionLayer(static_cast<uint16_t>(CollisionLayer::BurgerPlate));
    col->SetCollisionMask(BURGER_PLATE_COLLISION_MASK);
    col->SetCollisionType(CollisionType::Trigger);
    goTL->AddComponent<dae::PhysicsComponent>()->SetUseGravity(false);
    goTL->SetLocalPosition(startPos);
    dae::CollisionSystem::GetInstance().RegisterCollider(col);
    scene.Add(std::move(goTL));

    // Middle Left BurgerPlate
    auto goML = std::make_unique<dae::GameObject>(baseName + "_ML");
    auto* rendererCompML = goML->AddComponent<dae::RenderComponent>();
    rendererCompML->SetTexture("BurgerPlate_M_L.png");
    if (!hasScaledTextureML)
    {
        rendererCompML->Scale(2);
        hasScaledTextureML = true;
    }
    auto* colML =
        goML->AddComponent<dae::ColliderComponent>(glm::vec2{ G_SIZE, G_SIZE });
    colML->SetCollisionLayer(
        static_cast<uint16_t>(CollisionLayer::BurgerPlate));
    colML->SetCollisionMask(BURGER_PLATE_COLLISION_MASK);
    colML->SetCollisionType(CollisionType::Trigger);
    goML->AddComponent<dae::PhysicsComponent>()->SetUseGravity(false);
    goML->SetLocalPosition(startPos + glm::vec2(0, G_SIZE));
    dae::CollisionSystem::GetInstance().RegisterCollider(colML);
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
        auto* colM = goM->AddComponent<dae::ColliderComponent>(
            glm::vec2{ G_SIZE, G_SIZE });
        colM->SetCollisionLayer(
            static_cast<uint16_t>(CollisionLayer::BurgerPlate));
        colM->SetCollisionMask(BURGER_PLATE_COLLISION_MASK);
        colM->SetCollisionType(CollisionType::Trigger);
        goM->AddComponent<dae::PhysicsComponent>()->SetUseGravity(false);
        goM->SetLocalPosition(startPos + glm::vec2((i + 1) * G_SIZE, G_SIZE));
        dae::CollisionSystem::GetInstance().RegisterCollider(colM);
        scene.Add(std::move(goM));
    }

    static constexpr int PLATE_LENGTH = 5;
    // Middle Right BurgerPlate
    auto goMR = std::make_unique<dae::GameObject>(baseName + "_MR");
    auto* rendererCompMR = goMR->AddComponent<dae::RenderComponent>();
    rendererCompMR->SetTexture("BurgerPlate_M_R.png");
    if (!hasScaledTextureMR)
    {
        rendererCompMR->Scale(2);
        hasScaledTextureMR = true;
    }
    auto* colMR =
        goMR->AddComponent<dae::ColliderComponent>(glm::vec2{ G_SIZE, G_SIZE });
    colMR->SetCollisionLayer(
        static_cast<uint16_t>(CollisionLayer::BurgerPlate));
    colMR->SetCollisionMask(BURGER_PLATE_COLLISION_MASK);
    colMR->SetCollisionType(CollisionType::Trigger);
    goMR->AddComponent<dae::PhysicsComponent>()->SetUseGravity(false);
    goMR->SetLocalPosition(startPos + glm::vec2(PLATE_LENGTH * G_SIZE, G_SIZE));
    dae::CollisionSystem::GetInstance().RegisterCollider(colMR);
    scene.Add(std::move(goMR));

    // Top Right BurgerPlate
    auto goTR = std::make_unique<dae::GameObject>(baseName + "_TR");
    auto* rendererCompTR = goTR->AddComponent<dae::RenderComponent>();
    rendererCompTR->SetTexture("BurgerPlate_T_R.png");
    if (!hasScaledTextureTR)
    {
        rendererCompTR->Scale(2);
        hasScaledTextureTR = true;
    }
    auto* colTR =
        goTR->AddComponent<dae::ColliderComponent>(glm::vec2{ G_SIZE, G_SIZE });
    colTR->SetCollisionLayer(
        static_cast<uint16_t>(CollisionLayer::BurgerPlate));
    colTR->SetCollisionMask(BURGER_PLATE_COLLISION_MASK);
    colTR->SetCollisionType(CollisionType::Trigger);
    goTR->AddComponent<dae::PhysicsComponent>()->SetUseGravity(false);
    goTR->SetLocalPosition(startPos + glm::vec2(PLATE_LENGTH * G_SIZE, 0));
    dae::CollisionSystem::GetInstance().RegisterCollider(colTR);
    scene.Add(std::move(goTR));
}

static void CreateBurgerPart(dae::Scene& scene,
                             const glm::vec2& startPos,
                             const std::string& baseName)
{
    const std::vector<std::string> textureSuffixes = {
        "_L", "_M_L", "_M_R", "_R"
    };

    auto burgerGroupGo = std::make_unique<dae::GameObject>(baseName + "Group");
    auto* burgerGroup = burgerGroupGo->AddComponent<BurgerGroupComponent>();
    for (size_t i = 0; i < textureSuffixes.size(); ++i)
    {
        auto go =
            std::make_unique<dae::GameObject>(baseName + textureSuffixes[i]);
        auto* rendererComp = go->AddComponent<dae::RenderComponent>();
        rendererComp->SetTexture(baseName + textureSuffixes[i] + ".png");
        rendererComp->Scale(2);
        auto* collider = go->AddComponent<dae::ColliderComponent>(
            glm::vec2{ G_SIZE, G_SIZE });
        collider->SetCollisionLayer(
            static_cast<uint16_t>(CollisionLayer::BurgerPart));
        collider->SetCollisionMask(FOOD_COLLISION_MASK);
        collider->SetCollisionType(CollisionType::Trigger);
        go->AddComponent<dae::PhysicsComponent>()->SetUseGravity(false);
        go->AddComponent<BurgerPartComponent>(burgerGroup);
        go->SetLocalPosition(startPos + glm::vec2(i * G_SIZE, 0));

        dae::ServiceLocator::GetInstance().GetSoundSystem().Load(
            SoundIds::BurgerStep, "Sound/BurgerStep.wav");
        dae::CollisionSystem::GetInstance().RegisterCollider(collider);
        scene.Add(std::move(go));
    }
    scene.Add(std::move(burgerGroupGo));
}

static void SetupLevel0()
{
    static constexpr glm::vec2 COUNTER_POS = { G_SIZE * 2,
                                               G_SIZE * 5 + TOP_BORDER };

    auto& level0Scene = dae::SceneManager::GetInstance().CreateScene("Level0");

    auto go = std::make_unique<dae::GameObject>("LevelCounter");
    auto* renderComp = go->AddComponent<dae::RenderComponent>();
    renderComp->SetTexture("LevelCounter.png");
    renderComp->Scale(2);
    go->SetLocalPosition(COUNTER_POS);
    level0Scene.Add(std::move(go));
#pragma region FLOORS
    static constexpr glm::vec2 TOP_FLOOR_POS = { G_SIZE * 3,
                                                 G_SIZE * 5 + TOP_BORDER };
    static constexpr auto TOP_FLOOR_SEGMENTS = { 26 };
    static constexpr glm::vec2 MID_FLOOR_1_POS = { G_SIZE * 3,
                                                   G_SIZE * 9 + TOP_BORDER };
    static constexpr auto MID_FLOOR_1_SEGMENTS = { 8, -4, 14 };
    static constexpr glm::vec2 MID_FLOOR_2_POS = { G_SIZE * 3,
                                                   G_SIZE * 11 + TOP_BORDER };
    static constexpr auto MID_FLOOR_2_SEGMENTS = { -6, 8 };
    static constexpr glm::vec2 MID_FLOOR_3_POS = { G_SIZE * 3,
                                                   G_SIZE * 13 + TOP_BORDER };
    static constexpr auto MID_FLOOR_3_SEGMENTS = { 8, -10, 8 };
    static constexpr glm::vec2 MID_FLOOR_4_POS = { G_SIZE * 3,
                                                   G_SIZE * 15 + TOP_BORDER };
    static constexpr auto MID_FLOOR_4_SEGMENTS = { -6, 14 };
    static constexpr glm::vec2 MID_FLOOR_5_POS = { G_SIZE * 3,
                                                   G_SIZE * 17 + TOP_BORDER };
    static constexpr auto MID_FLOOR_5_SEGMENTS = { -18, 8 };
    static constexpr glm::vec2 MID_FLOOR_6_POS = { G_SIZE * 3,
                                                   G_SIZE * 19 + TOP_BORDER };
    static constexpr auto MID_FLOOR_6_SEGMENTS = { 20 };
    static constexpr glm::vec2 BOTTOM_FLOOR_POS = { G_SIZE * 3,
                                                    G_SIZE * 23 + TOP_BORDER };
    static constexpr auto BOTTOM_FLOOR_SEGMENTS = { 26 };

    CreateFloorPlane(
        level0Scene, TOP_FLOOR_POS, TOP_FLOOR_SEGMENTS, "TopFloor");

    CreateFloorPlane(
        level0Scene, MID_FLOOR_1_POS, MID_FLOOR_1_SEGMENTS, "MidFloor1");
    CreateFloorPlane(
        level0Scene, MID_FLOOR_2_POS, MID_FLOOR_2_SEGMENTS, "MidFloor2");
    CreateFloorPlane(
        level0Scene, MID_FLOOR_3_POS, MID_FLOOR_3_SEGMENTS, "MidFloor3");
    CreateFloorPlane(
        level0Scene, MID_FLOOR_4_POS, MID_FLOOR_4_SEGMENTS, "MidFloor4");
    CreateFloorPlane(
        level0Scene, MID_FLOOR_5_POS, MID_FLOOR_5_SEGMENTS, "MidFloor5");
    CreateFloorPlane(
        level0Scene, MID_FLOOR_6_POS, MID_FLOOR_6_SEGMENTS, "MidFloor6");

    CreateFloorPlane(
        level0Scene, BOTTOM_FLOOR_POS, BOTTOM_FLOOR_SEGMENTS, "BottomFloor");

#pragma endregion
#pragma region LADDERS
    static constexpr glm::vec2 LADDER_1_POS{ G_SIZE * 3,
                                             G_SIZE * 5 + TOP_BORDER };
    static constexpr int LADDER_1_COUNT{ 4 };
    static constexpr bool LADDER_1_GREEN{ true };
    static constexpr glm::vec2 LADDER_2_POS{ G_SIZE * 3,
                                             G_SIZE * 13 + TOP_BORDER };
    static constexpr int LADDER_2_COUNT{ 10 };
    static constexpr bool LADDER_2_GREEN{ true };
    static constexpr glm::vec2 LADDER_3_POS{ G_SIZE * 6,
                                             G_SIZE * 9 + TOP_BORDER };
    static constexpr int LADDER_3_COUNT{ 10 };
    static constexpr bool LADDER_3_GREEN{ false };
    static constexpr glm::vec2 LADDER_4_POS{ G_SIZE * 9,
                                             G_SIZE * 5 + TOP_BORDER };
    static constexpr int LADDER_4_COUNT{ 18 };
    static constexpr bool LADDER_4_GREEN{ true };
    static constexpr glm::vec2 LADDER_5_POS{ G_SIZE * 12,
                                             G_SIZE * 5 + TOP_BORDER };
    static constexpr int LADDER_5_COUNT{ 6 };
    static constexpr bool LADDER_5_GREEN{ false };
    static constexpr glm::vec2 LADDER_6_POS{ G_SIZE * 15,
                                             G_SIZE * 5 + TOP_BORDER };
    static constexpr int LADDER_6_COUNT{ 18 };
    static constexpr bool LADDER_6_GREEN{ true };
    static constexpr glm::vec2 LADDER_7_POS{ G_SIZE * 18,
                                             G_SIZE * 9 + TOP_BORDER };
    static constexpr int LADDER_7_COUNT{ 6 };
    static constexpr bool LADDER_7_GREEN{ false };
    static constexpr glm::vec2 LADDER_8_POS{ G_SIZE * 21,
                                             G_SIZE * 5 + TOP_BORDER };
    static constexpr int LADDER_8_COUNT{ 18 };
    static constexpr bool LADDER_8_GREEN{ true };
    static constexpr glm::vec2 LADDER_9_POS{ G_SIZE * 24,
                                             G_SIZE * 13 + TOP_BORDER };
    static constexpr int LADDER_9_COUNT{ 10 };
    static constexpr bool LADDER_9_GREEN{ false };
    static constexpr glm::vec2 LADDER_10_POS{ G_SIZE * 27,
                                              G_SIZE * 5 + TOP_BORDER };
    static constexpr int LADDER_10_COUNT{ 8 };
    static constexpr bool LADDER_10_GREEN{ true };
    static constexpr glm::vec2 LADDER_11_POS{ G_SIZE * 27,
                                              G_SIZE * 17 + TOP_BORDER };
    static constexpr int LADDER_11_COUNT{ 6 };
    static constexpr bool LADDER_11_GREEN{ true };

    CreateLadder(level0Scene, LADDER_1_POS, LADDER_1_COUNT, LADDER_1_GREEN);
    CreateLadder(level0Scene, LADDER_2_POS, LADDER_2_COUNT, LADDER_2_GREEN);
    CreateLadder(level0Scene, LADDER_3_POS, LADDER_3_COUNT, LADDER_3_GREEN);
    CreateLadder(level0Scene, LADDER_4_POS, LADDER_4_COUNT, LADDER_4_GREEN);
    CreateLadder(level0Scene, LADDER_5_POS, LADDER_5_COUNT, LADDER_5_GREEN);
    CreateLadder(level0Scene, LADDER_6_POS, LADDER_6_COUNT, LADDER_6_GREEN);
    CreateLadder(level0Scene, LADDER_7_POS, LADDER_7_COUNT, LADDER_7_GREEN);
    CreateLadder(level0Scene, LADDER_8_POS, LADDER_8_COUNT, LADDER_8_GREEN);
    CreateLadder(level0Scene, LADDER_9_POS, LADDER_9_COUNT, LADDER_9_GREEN);
    CreateLadder(level0Scene, LADDER_10_POS, LADDER_10_COUNT, LADDER_10_GREEN);
    CreateLadder(level0Scene, LADDER_11_POS, LADDER_11_COUNT, LADDER_11_GREEN);
#pragma endregion
#pragma region BURGERPLATE
    static constexpr glm::vec2 BURGERPLATE_POS_1 = { G_SIZE * 4, G_SIZE * 27 };
    static constexpr glm::vec2 BURGERPLATE_POS_2 = { G_SIZE * 10, G_SIZE * 27 };
    static constexpr glm::vec2 BURGERPLATE_POS_3 = { G_SIZE * 16, G_SIZE * 27 };
    static constexpr glm::vec2 BURGERPLATE_POS_4 = { G_SIZE * 22, G_SIZE * 27 };

    CreateBurgerPlateStack(level0Scene, BURGERPLATE_POS_1);
    CreateBurgerPlateStack(level0Scene, BURGERPLATE_POS_2);
    CreateBurgerPlateStack(level0Scene, BURGERPLATE_POS_3);
    CreateBurgerPlateStack(level0Scene, BURGERPLATE_POS_4);
#pragma endregion
#pragma region FOOD
    static constexpr auto FOOD_TYPE_BUN_TOP = "BurgerBunTop";
    static constexpr glm::vec2 BUN_TOP_POS_1 = { glm::ivec2{ G_SIZE * 5,
                                                             G_SIZE * 9 - 2 } };
    static constexpr glm::vec2 BUN_TOP_POS_2 = { glm::ivec2{ G_SIZE * 11,
                                                             G_SIZE * 5 - 2 } };
    static constexpr glm::vec2 BUN_TOP_POS_3 = { glm::ivec2{ G_SIZE * 17,
                                                             G_SIZE * 5 - 2 } };
    static constexpr glm::vec2 BUN_TOP_POS_4 = { glm::ivec2{ G_SIZE * 23,
                                                             G_SIZE * 5 - 2 } };
    static constexpr auto FOOD_TYPE_LETTUCE = "Lettuce";
    static constexpr glm::vec2 LETTUCE_POS_1 = { glm::ivec2{
        G_SIZE * 5, G_SIZE * 13 - 2 } };
    static constexpr glm::vec2 LETTUCE_POS_2 = { glm::ivec2{
        G_SIZE * 11, G_SIZE * 15 - 2 } };
    static constexpr glm::vec2 LETTUCE_POS_3 = { glm::ivec2{ G_SIZE * 17,
                                                             G_SIZE * 9 - 2 } };
    static constexpr glm::vec2 LETTUCE_POS_4 = { glm::ivec2{ G_SIZE * 23,
                                                             G_SIZE * 9 - 2 } };
    static constexpr auto FOOD_TYPE_BURGER = "Burger";
    static constexpr glm::vec2 BURGER_POS_1 = { glm::ivec2{ G_SIZE * 5,
                                                            G_SIZE * 19 - 2 } };
    static constexpr glm::vec2 BURGER_POS_2 = { glm::ivec2{ G_SIZE * 11,
                                                            G_SIZE * 19 - 2 } };
    static constexpr glm::vec2 BURGER_POS_3 = { glm::ivec2{ G_SIZE * 17,
                                                            G_SIZE * 15 - 2 } };
    static constexpr glm::vec2 BURGER_POS_4 = { glm::ivec2{ G_SIZE * 23,
                                                            G_SIZE * 13 - 2 } };
    static constexpr auto FOOD_TYPE_BUN_BOTTOM = "BurgerBunBottom";
    static constexpr glm::vec2 BUN_BOTTOM_POS_1 = { glm::ivec2{
        G_SIZE * 5, G_SIZE * 23 - 2 } };
    static constexpr glm::vec2 BUN_BOTTOM_POS_2 = { glm::ivec2{
        G_SIZE * 11, G_SIZE * 23 - 2 } };
    static constexpr glm::vec2 BUN_BOTTOM_POS_3 = { glm::ivec2{
        G_SIZE * 17, G_SIZE * 23 - 2 } };
    static constexpr glm::vec2 BUN_BOTTOM_POS_4 = { glm::ivec2{
        G_SIZE * 23, G_SIZE * 17 - 2 } };

    CreateBurgerPart(level0Scene, BUN_TOP_POS_1, FOOD_TYPE_BUN_TOP);
    CreateBurgerPart(level0Scene, BUN_TOP_POS_2, FOOD_TYPE_BUN_TOP);
    CreateBurgerPart(level0Scene, BUN_TOP_POS_3, FOOD_TYPE_BUN_TOP);
    CreateBurgerPart(level0Scene, BUN_TOP_POS_4, FOOD_TYPE_BUN_TOP);
    CreateBurgerPart(level0Scene, LETTUCE_POS_1, FOOD_TYPE_LETTUCE);
    CreateBurgerPart(level0Scene, LETTUCE_POS_2, FOOD_TYPE_LETTUCE);
    CreateBurgerPart(level0Scene, LETTUCE_POS_3, FOOD_TYPE_LETTUCE);
    CreateBurgerPart(level0Scene, LETTUCE_POS_4, FOOD_TYPE_LETTUCE);
    CreateBurgerPart(level0Scene, BURGER_POS_1, FOOD_TYPE_BURGER);
    CreateBurgerPart(level0Scene, BURGER_POS_2, FOOD_TYPE_BURGER);
    CreateBurgerPart(level0Scene, BURGER_POS_3, FOOD_TYPE_BURGER);
    CreateBurgerPart(level0Scene, BURGER_POS_4, FOOD_TYPE_BURGER);
    CreateBurgerPart(level0Scene, BUN_BOTTOM_POS_1, FOOD_TYPE_BUN_BOTTOM);
    CreateBurgerPart(level0Scene, BUN_BOTTOM_POS_2, FOOD_TYPE_BUN_BOTTOM);
    CreateBurgerPart(level0Scene, BUN_BOTTOM_POS_3, FOOD_TYPE_BUN_BOTTOM);
    CreateBurgerPart(level0Scene, BUN_BOTTOM_POS_4, FOOD_TYPE_BUN_BOTTOM);
#pragma endregion
#pragma region AUDIO
    static constexpr float BGM_VOlUME = 0.1f;
    auto& soundSystem = dae::ServiceLocator::GetSoundSystem();
    soundSystem.Load(SoundIds::Bgm, "Sound/BGM.wav");
    soundSystem.Play(SoundIds::Bgm, BGM_VOlUME, true);
#pragma endregion
    LevelGrid::GetInstance().InitializeLevelGrid(level0Scene, GRID_OFFSET);
}

static void load()
{
    SetupLevel0();
    SetupPlayers();
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
    engine.Run([]() { load(); });
    return 0;
}
