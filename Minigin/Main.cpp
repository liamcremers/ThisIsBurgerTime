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

	auto ThrashTheCache = std::make_unique<dae::GameObject>();
	ThrashTheCache->AddComponent<dae::ThrashTheCacheComponent>(windowWidth, windowHeight);
	scene.Add(std::move(ThrashTheCache));
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
