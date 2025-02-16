#pragma once
namespace dae
{
	class GameObject;
	class BaseComponent
	{
	public:
		BaseComponent(GameObject* parent) : m_ParentGameObjectPtr(parent) {}
		virtual void Update() {};
		virtual void FixedUpdate() {};
		virtual void Render() const {};
		virtual void LateUpdate() {};

		virtual [[nodiscard]] GameObject* GetGameObject() const { return m_ParentGameObjectPtr; }
	private:
		GameObject* m_ParentGameObjectPtr{};
	};
}
