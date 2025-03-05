#include "SceneManager.h"
#include "Scene.h"

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
    const auto& scene = std::shared_ptr<Scene>(new Scene(name));
    m_Scenes.push_back(scene);
    return *scene;
}
