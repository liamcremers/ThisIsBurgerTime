#include "Scene.h"
#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(std::string name) :
    m_name(std::move(name))
{}

void Scene::Add(std::unique_ptr<GameObject> object)
{
    m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::unique_ptr<GameObject> object)
{
    auto it = std::ranges::find(m_objects, object);
    if (it != m_objects.end())
    {
        m_objects.erase(it);
    }
}

void Scene::RemoveAll() { m_objects.clear(); }

void Scene::Update()
{
    for (auto& object : m_objects)
    {
        object->Update();
    }
}

void dae::Scene::FixedUpdate()
{
    for (auto& object : m_objects)
    {
        object->FixedUpdate();
    }
}

void Scene::Render() const
{
    for (auto& object : m_objects)
    {
        object->Render();
    }
}

void dae::Scene::LateUpdate()
{
    for (auto& object : m_objects)
        object->LateUpdate();

    std::erase_if(m_objects,
                  [](const std::unique_ptr<GameObject>& object)
                  { return object->IsMarkedForDelete(); });
}
