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

// TODO: Missing something on line 43 in GameObject.cpp - check with the slides
// TODO: No need for m_Origin
static void load(int windowWidth, int windowHeight)
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_unique<dae::GameObject>();
	auto meshRenderer = go->AddComponent<dae::RenderComponent>();
	meshRenderer->SetTexture("background.tga");
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	meshRenderer = go->AddComponent<dae::RenderComponent>();
	meshRenderer->SetTexture("logo.tga");
	go->GetComponent<dae::TransformComponent>()->SetWorldPosition({ 216.0f, 180.0f });
	scene.Add(std::move(go));

	auto& font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	go->GetComponent<dae::TransformComponent>()->SetWorldPosition({ 80.0f, 20.0f });
	scene.Add(std::move(go));

	auto parent = std::make_unique<dae::GameObject>("parent");
	glm::vec2 pos = { windowHeight / 2.0f, windowWidth / 2.0f };
	parent->GetComponent<dae::TransformComponent>()->SetWorldPosition({ pos });

	auto child1 = std::make_unique<dae::GameObject>("parent");
	meshRenderer = child1->AddComponent<dae::RenderComponent>();
	meshRenderer->SetTexture("ChefPeterPepperF.png");
	child1->GetComponent<dae::TransformComponent>()->SetWorldPosition({ pos });
	child1->SetParent(parent.get(), true);
	child1->AddComponent<dae::RotatorComponent>(40, true);

	auto child2 = std::make_unique<dae::GameObject>("child");
	glm::vec2 newpos = pos + glm::vec2{ 20,20 };
	meshRenderer = child2->AddComponent<dae::RenderComponent>();
	meshRenderer->SetTexture("ChefPeterPepperB.png");
	child2->GetComponent<dae::TransformComponent>()->SetWorldPosition({ newpos });
	child2->SetParent(child1.get(), false);
	child2->AddComponent<dae::RotatorComponent>(40, false);

	scene.Add(std::move(parent));
	scene.Add(std::move(child1));
	scene.Add(std::move(child2));

	auto& fpsFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	go = std::make_unique<dae::GameObject>("fps");
	go->AddComponent<dae::FPSComponent>(fpsFont);
	go->GetComponent<dae::TransformComponent>()->SetWorldPosition({ 10.0f, 6.0f });
	scene.Add(std::move(go));
}

int main(int, char* [])
{
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if (!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run([&engine]() { load(engine.GetWindowWidth(), engine.GetWindowHeight()); });
	return 0;
}
