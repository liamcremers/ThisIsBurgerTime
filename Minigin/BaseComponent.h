#pragma once
namespace dae
{
	class GameObject;
	class BaseComponent
	{
	public:
		BaseComponent(GameObject* parent) : m_ParentGameObjectPtr(parent) {}
		virtual ~BaseComponent() = default;

		virtual void Update() = 0;
		virtual void Render() const = 0;

		virtual [[nodiscard]] GameObject* GetGameObject() const { return m_ParentGameObjectPtr; }
	private:
		GameObject* m_ParentGameObjectPtr{};
	};
}
