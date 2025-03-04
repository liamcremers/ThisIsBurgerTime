#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

namespace dae
{
    class Scene;
    class SceneManager final : public Singleton<SceneManager>
    {
    public:
        Scene& CreateScene(const std::string& name);

        void FixedUpdate();
        void Update();
        void Render();
        void LateUpdate();

    private:
        friend class Singleton<SceneManager>;
        std::vector<std::shared_ptr<Scene>> m_scenes;
    };
}
