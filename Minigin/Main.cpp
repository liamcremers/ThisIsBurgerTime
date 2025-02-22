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

static void load()
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

	go = std::make_unique<dae::GameObject>("fps");
	go->AddComponent<dae::FPSComponent>();
	go->GetComponent<dae::TransformComponent>()->SetWorldPosition({10.0f, 10.0f});
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
	engine.Run(load);
	return 0;
}
