#include "GameObject.h"

dae::GameObject::GameObject()
{
	AddComponent<TransformComponent>();
}

dae::GameObject::GameObject(const std::string& Name) : m_Name(Name)
{
	AddComponent<TransformComponent>();
}

void dae::GameObject::Update() {}

void dae::GameObject::FixedUpdate()
{}

void dae::GameObject::LateUpdate()
{}

void dae::GameObject::Render() const
{
	for (auto& comp : m_pComponents)
	{
		comp->Render();
	}
}

void dae::GameObject::SetParent(GameObject* pParent)
{
	if (m_pParent.get() == pParent)
		return;

	// Remove from current parent
	if (m_pParent)
	{
		auto it = std::find_if(m_pParent->m_pChildren.begin(), m_pParent->m_pChildren.end(),
							   [this](const std::unique_ptr<GameObject>& child) { return child.get() == this; });
		if (it != m_pParent->m_pChildren.end())
		{
			m_pParent->m_pChildren.erase(it);
		}
	}

	// Set new parent
	m_pParent.reset(pParent);
	if (m_pParent)
	{
		m_pParent->m_pChildren.emplace_back(this);
	}
}

dae::GameObject* dae::GameObject::GetParent() const { return m_pParent.get(); }

size_t dae::GameObject::GetChildCount() const { return m_pChildren.size(); }

dae::GameObject* dae::GameObject::GetChildAt(size_t idx) const { return m_pChildren.at(idx).get(); }

void dae::GameObject::MarkForDelete()
{
	m_MarkedForDelete = true;
	for (auto& child : m_pChildren)
	{
		child->MarkForDelete();
	}
}

bool dae::GameObject::IsMarkedForDelete() const
{
	return m_MarkedForDelete;
}

bool dae::GameObject::HasNoComponents() const
{
	return m_pComponents.empty();
}
void dae::GameObject::SetPosition(const glm::vec2& pos) const
{
	GetComponent<TransformComponent>()->SetPosition(pos);
}

void dae::GameObject::SetPosition(const float x, const float y) const
{
	GetComponent<TransformComponent>()->SetPosition(x, y);
}

const glm::vec2& dae::GameObject::GetPosition() const
{
	return GetComponent<TransformComponent>()->GetPosition();
}

const std::string& dae::GameObject::GetName() const
{
	return m_Name;
}
