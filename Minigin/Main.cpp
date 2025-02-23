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
namespace fs = std::filesystem;

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

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	go->GetComponent<dae::TransformComponent>()->SetWorldPosition({ 80.0f, 20.0f });
	scene.Add(std::move(go));

	auto parent = std::make_unique<dae::GameObject>("parent");
	glm::vec2 pos = { windowHeight / 2.0f, windowWidth / 2.0f };
	meshRenderer = parent->AddComponent<dae::RenderComponent>();
	meshRenderer->SetTexture("ChefPeterPepperF.png");
	parent->GetComponent<dae::TransformComponent>()->SetWorldPosition({ pos });
	parent->AddComponent<dae::RotatorComponent>(40);
	//moved after adding children

	auto child = std::make_unique<dae::GameObject>("child");
	meshRenderer = child->AddComponent<dae::RenderComponent>();
	meshRenderer->SetTexture("ChefPeterPepperB.png");
	child->GetComponent<dae::TransformComponent>()->SetLocalPosition(glm::vec2{ 20.0f, 20.0f });
	child->AddComponent<dae::RotatorComponent>(40, false);
	child->SetParent(parent.get());

	scene.Add(std::move(parent));
	scene.Add(std::move(child));
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
