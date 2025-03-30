#include "SceneManager.h"
#include "Scene.h"
#include <cassert>
#include <algorithm>

void dae::SceneManager::Update()
{
    for (auto& scene : m_Scenes)
    {
        scene->Update();
    }
}

void dae::SceneManager::FixedUpdate()
{
    for (const auto& scene : m_Scenes)
    {
        scene->FixedUpdate();
    }
}

void dae::SceneManager::Render()
{
    for (const auto& scene : m_Scenes)
    {
        scene->Render();
    }
}

void dae::SceneManager::LateUpdate()
{
    for (auto& scene : m_Scenes)
    {
        scene->LateUpdate();
    }
}

auto dae::SceneManager::CreateScene(const std::string& name) -> dae::Scene&
{
    assert(!name.empty() && "Scene name cannot be empty");
    assert(std::ranges::none_of(m_Scenes,

                                [&name](const std::shared_ptr<Scene>& scene)
                                { return scene->GetName() == name; }) &&
           "Scene with name already exists");

    const auto& scene = std::shared_ptr<Scene>(new Scene(name));
    m_Scenes.push_back(scene);
    return *scene;
}
