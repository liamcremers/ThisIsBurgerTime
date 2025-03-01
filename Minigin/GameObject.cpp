#include "GameObject.h"
//TODO: remove
#include "Windows.h"

dae::GameObject::GameObject(const std::string& Name)
	: m_Name(Name)
	, m_pTransform(AddComponent<TransformComponent>())
{}

void dae::GameObject::Update()
{
	for (auto& comp : m_pComponents)
		comp->Update();
}
void dae::GameObject::FixedUpdate() { for (auto& comp : m_pComponents) comp->FixedUpdate(); }
void dae::GameObject::LateUpdate()
{
	for (auto& comp : m_pComponents)
		comp->LateUpdate();
	for (auto& child : m_pChildren)
		child->LateUpdate();

	m_pChildren.erase(
		std::remove_if(m_pChildren.begin(), m_pChildren.end(),
		[](GameObject* child) { return child->IsMarkedForDelete(); }),
		m_pChildren.end());
}
void dae::GameObject::Render() const { for (auto& comp : m_pComponents) comp->Render(); }

void dae::GameObject::SetParent(GameObject* pParent, bool keepWorldPosition)
{
	if (pParent == this || pParent == m_pParent || IsChild(pParent))
		return;

	if (pParent == nullptr)
		SetLocalPosition(GetWorldPosition());
	else
	{
		if (keepWorldPosition)
			SetLocalPosition(GetWorldPosition() - pParent->GetWorldPosition());
		else
			SetPositionDirty();
	}

	if (m_pParent) m_pParent->RemoveChild(this);
	m_pParent = pParent;
	if (pParent) pParent->AddChild(this);
}

constexpr bool dae::GameObject::IsChild(const GameObject* pChild) const
{
	return std::find(m_pChildren.begin(), m_pChildren.end(), pChild) != m_pChildren.end();
}

const glm::vec2& dae::GameObject::GetWorldPosition()
{
	if (m_PositionDirty)
		UpdateWorldPosition();
	return m_pTransform->GetWorldPosition();
}

void dae::GameObject::UpdateWorldPosition()
{
	if (m_PositionDirty)
	{
		if (m_pParent)
			m_pTransform->SetWorldPosition(m_pParent->GetWorldPosition() + m_pTransform->GetLocalPosition());
		else
			m_pTransform->SetWorldPosition(m_pTransform->GetLocalPosition());

		OutputDebugStringA((m_Name + " " + std::to_string(m_pTransform->GetWorldPosition().x) + " " + std::to_string(m_pTransform->GetWorldPosition().y) + "\n").c_str());

		m_PositionDirty = false;
	}
}

void dae::GameObject::MarkForDelete()
{
	m_MarkedForDelete = true;
	for (auto& child : m_pChildren)
	{
		child->MarkForDelete();
	}
}

const glm::vec2& dae::GameObject::GetLocalPosition() const
{
	return m_pTransform->GetLocalPosition();
}

void dae::GameObject::SetLocalPosition(const glm::vec2& position)
{
	m_pTransform->SetLocalPosition(position);
	SetPositionDirty();
}

void dae::GameObject::SetPositionDirty()
{
	m_PositionDirty = true;

	for (auto& child : m_pChildren)
		child->SetPositionDirty();
};

void dae::GameObject::RemoveChild(GameObject* pChild)
{
	std::erase_if(m_pChildren, std::bind_front(std::equal_to<>(), pChild));
}

void dae::GameObject::AddChild(GameObject* pChild)
{
	m_pChildren.emplace_back(pChild);
}
