#pragma once

namespace dae
{
    class GameObject;

    class BaseComponent
    {
    public:
        virtual void Update() {};
        virtual void FixedUpdate() {};
        virtual void Render() const {};
        virtual void LateUpdate() {};

        virtual ~BaseComponent() = default;
        BaseComponent(const BaseComponent& other) = delete;
        BaseComponent(BaseComponent&& other) = delete;
        BaseComponent& operator=(const BaseComponent& other) = delete;
        BaseComponent& operator=(BaseComponent&& other) = delete;

    protected:
        explicit BaseComponent(GameObject& owner);
        [[nodiscard]] virtual auto GetOwner() const -> GameObject& final;

    private:
        GameObject& m_pOwner;
    };
}
