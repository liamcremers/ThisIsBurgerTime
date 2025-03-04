#include "GameObject.h"

dae::GameObject::GameObject(std::string Name) :
    m_Name(std::move(Name)),
    m_pTransform(AddComponent<TransformComponent>())
{}

void dae::GameObject::Update()
{
    for (auto& comp : m_pComponents)
        comp->Update();
}

void dae::GameObject::FixedUpdate()
{
    for (auto& comp : m_pComponents)
        comp->FixedUpdate();
}

void dae::GameObject::LateUpdate()
{
    for (auto& comp : m_pComponents)
        comp->LateUpdate();
    for (auto& child : m_pChildren)
        child->LateUpdate();

    std::erase_if(m_pChildren,
                  [](GameObject* child) { return child->IsMarkedForDelete(); });
}

void dae::GameObject::Render() const
{
    for (auto& comp : m_pComponents)
        comp->Render();
}

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

    if (m_pParent)
        m_pParent->RemoveChild(this);
    m_pParent = pParent;
    if (pParent)
        pParent->AddChild(this);
}

auto dae::GameObject::IsChild(const GameObject* pChild) const -> bool
{
    return std::ranges::find(m_pChildren, pChild) != m_pChildren.end();
}

auto dae::GameObject::GetWorldPosition() -> const glm::vec2&
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
            m_pTransform->SetWorldPosition(m_pParent->GetWorldPosition() +
                                           m_pTransform->GetLocalPosition());
        else
            m_pTransform->SetWorldPosition(m_pTransform->GetLocalPosition());

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

auto dae::GameObject::GetLocalPosition() const -> const glm::vec2&
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
