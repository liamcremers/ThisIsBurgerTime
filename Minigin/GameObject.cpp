#include "GameObject.h"

dae::GameObject::GameObject(const std::string& Name)
	: m_Name(Name)
{
	AddComponent<TransformComponent>();
}

void dae::GameObject::Update() { for (auto& comp : m_pComponents) comp->Update(); }
void dae::GameObject::FixedUpdate() { for (auto& comp : m_pComponents) comp->FixedUpdate(); }
void dae::GameObject::LateUpdate()
{
	for (auto& comp : m_pComponents) 
		comp->LateUpdate();
	for (auto& child : m_pChildren)
		child->LateUpdate();

	m_pChildren.erase(
		std::remove_if(m_pChildren.begin(), m_pChildren.end(),
		[](const std::unique_ptr<GameObject>& child) { return child->IsMarkedForDelete(); }),
		m_pChildren.end());
}
void dae::GameObject::Render() const { for (auto& comp : m_pComponents) comp->Render(); }

void dae::GameObject::SetParent(GameObject* pParent)
{
	if (m_pParent.get() == pParent)
		return;

	// Remove from current parent
	if (m_pParent)
	{
		m_pParent->m_pChildren.erase(
			std::remove_if(m_pParent->m_pChildren.begin(), m_pParent->m_pChildren.end(),
			[this](const std::unique_ptr<GameObject>& child) { return child.get() == this; }),
			m_pParent->m_pChildren.end());
	}

	// Set new parent
	m_pParent.reset(pParent);
	m_pParent->m_pChildren.emplace_back(this);
}

void dae::GameObject::MarkForDelete()
{
	m_MarkedForDelete = true;
	for (auto& child : m_pChildren)
	{
		child->MarkForDelete();
	}
}
