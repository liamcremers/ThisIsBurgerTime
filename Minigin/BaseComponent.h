#pragma once
namespace dae
{
	class GameObject;
	class BaseComponent
	{
	public:
		virtual ~BaseComponent() = default;
		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

		virtual void Update() {};
		virtual void FixedUpdate() {};
		virtual void Render() const {};
		virtual void LateUpdate() {};
	protected:
		BaseComponent(GameObject& owner) : m_pOwner(&owner) {}
		virtual [[nodiscard]] GameObject* GetOwner() const { return m_pOwner; }
	private:
		GameObject* m_pOwner;
		// TODO: question? GameObject& possible fix?
	};
}
