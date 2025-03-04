#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "ResourceManager.h"
#include "Scene.h"

#include "ComponentsHeader.h"

#include <filesystem>
#include <memory>
namespace fs = std::filesystem;

static constexpr float RADIUS = 40.0f;
static constexpr uint8_t SMALL_FONT_SIZE = 14;
static constexpr float FPS_POS_X = 10.0f;
static constexpr float FPS_POS_Y = 6.0f;
static constexpr float LOGO_POS_X = 216.0f;
static constexpr float LOGO_POS_Y = 180.0f;
static constexpr uint8_t LARGE_FONT_SIZE = 36;
static constexpr float TEXT_POS_X = 80.0f;
static constexpr float TEXT_POS_Y = 20.0f;
static constexpr float OFFSET = 20.0f;

static void load(int windowWidth, int windowHeight)
{
    auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

    auto go = std::make_unique<dae::GameObject>();
    auto meshRenderer = go->AddComponent<dae::RenderComponent>();
    meshRenderer->SetTexture("background.tga");
    scene.Add(std::move(go));

    const glm::vec2 logoPos{ LOGO_POS_X, LOGO_POS_Y };
    go = std::make_unique<dae::GameObject>();
    meshRenderer = go->AddComponent<dae::RenderComponent>();
    meshRenderer->SetTexture("logo.tga");
    go->GetComponent<dae::TransformComponent>()->SetWorldPosition(logoPos);
    scene.Add(std::move(go));

    const glm::vec2 textPos{ TEXT_POS_X, TEXT_POS_Y };
    auto& font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf",
                                                              LARGE_FONT_SIZE);
    go = std::make_unique<dae::GameObject>();
    go->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
    go->GetComponent<dae::TransformComponent>()->SetWorldPosition(textPos);
    scene.Add(std::move(go));

    const glm::vec2 pos{ static_cast<float>(windowHeight) / 2.0f,
                         static_cast<float>(windowWidth) / 2.0f };
    auto parent = std::make_unique<dae::GameObject>("parent");
    parent->GetComponent<dae::TransformComponent>()->SetWorldPosition(pos);

    bool clockwise{ true };
    auto child1 = std::make_unique<dae::GameObject>("parent");
    meshRenderer = child1->AddComponent<dae::RenderComponent>();
    meshRenderer->SetTexture("ChefPeterPepperF.png");
    child1->GetComponent<dae::TransformComponent>()->SetWorldPosition(pos);
    child1->SetParent(parent.get(), true);
    child1->AddComponent<dae::RotatorComponent>(RADIUS, clockwise);

    bool clockwise2{ false };
    auto child2{ std::make_unique<dae::GameObject>("child") };
    const glm::vec2 newpos{ pos + glm::vec2{ OFFSET, OFFSET } };
    meshRenderer = child2->AddComponent<dae::RenderComponent>();
    meshRenderer->SetTexture("ChefPeterPepperB.png");
    child2->GetComponent<dae::TransformComponent>()->SetWorldPosition(newpos);
    child2->SetParent(child1.get(), false);
    child2->AddComponent<dae::RotatorComponent>(RADIUS, clockwise2);

    scene.Add(std::move(parent));
    scene.Add(std::move(child1));
    scene.Add(std::move(child2));

    auto& fpsFont = dae::ResourceManager::GetInstance().LoadFont(
        "Lingua.otf", SMALL_FONT_SIZE);
    go = std::make_unique<dae::GameObject>("fps");
    go->AddComponent<dae::FPSComponent>(fpsFont);
    go->GetComponent<dae::TransformComponent>()->SetWorldPosition(
        { FPS_POS_X, FPS_POS_Y });
    scene.Add(std::move(go));
}

int main(int, char*[])
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
