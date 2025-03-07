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
        BaseComponent(GameObject& owner) :
            m_pOwner(owner)
        {}

        [[nodiscard]] virtual GameObject& GetOwner() const final;

    private:
        GameObject& m_pOwner;
    };
}
