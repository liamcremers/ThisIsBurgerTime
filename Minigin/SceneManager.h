#pragma once
#include "Singleton.h"

#include <vector>
#include <string>
#include <memory>

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
        SceneManager() = default;

        std::vector<std::shared_ptr<Scene>> m_Scenes{};
    };
}
