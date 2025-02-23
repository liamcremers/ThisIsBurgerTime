#pragma once
#include "BaseComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "ComponentsHeader.h"

#include <string>
#include <glm.hpp>
#include <memory>
#include <vector>
#include <optional>
#include <algorithm>
#include <concepts>
#include <functional>
#include <ranges>

namespace dae
{
	template<typename CompT>
	concept Component = std::derived_from<CompT, BaseComponent>;

	class Texture2D;

	class GameObject final
	{
	public:
		GameObject(const std::string& Name = {});

		GameObject(const GameObject&) = delete;
		GameObject(GameObject&&) = delete;
		GameObject& operator=(const GameObject&) = delete;
		GameObject& operator=(GameObject&&) = delete;

		// Core functions
		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;

		// Parent & Children
		void SetParent(GameObject* pParent, bool keepWorldPosition = false);
		void DetechFromParent() { SetParent(nullptr); }
		[[nodiscard]] bool IsChild(GameObject* pChild) const;
		[[nodiscard]] GameObject* GetParent() const { return m_pParent; }
		[[nodiscard]] size_t GetChildCount() const { return m_pChildren.size(); }
		[[nodiscard]] GameObject* GetChildAt(size_t idx) const { return m_pChildren.at(idx); }

		[[nodiscard]] const glm::vec2& GetWorldPosition();
		[[nodiscard]] const glm::vec2& GetLocalPosition() const;
		void UpdateWorldPosition();
		void SetLocalPosition(const glm::vec2& position);
		void SetPositionDirty();


		// Object state management
		void MarkForDelete();
		[[nodiscard]] auto IsMarkedForDelete() const -> bool { return m_MarkedForDelete; }
		[[nodiscard]] auto HasNoComponents() const -> bool { return m_pComponents.empty(); }

		[[nodiscard]] const std::string& GetName() const { return m_Name; }

		// Component management
		template<Component CompT, typename... Args>
		auto AddComponent(Args&&... args) -> CompT*
		{
			if (HasComponent<CompT>())
				RemoveComponent<CompT>();

			auto component = std::make_unique<CompT>(*this, std::forward<Args>(args)...);
			auto* componentPtr = component.get();
			m_pComponents.emplace_back(std::move(component));
			return componentPtr;
		}

		template<Component CompT>
		void RemoveComponent()
		{
			std::erase_if(m_pComponents, [](const auto& pComp)
						  {
							  return dynamic_cast<CompT*>(pComp.get()) != nullptr;
						  });
		}

		template<Component CompT>
		[[nodiscard]] constexpr bool HasComponent() const
		{
			return std::ranges::any_of(m_pComponents, [](const auto& pComp)
									   {
										   return dynamic_cast<CompT*>(pComp.get()) != nullptr;
									   });
		}

		template<Component CompT>
		[[nodiscard]] constexpr auto GetComponent() const -> CompT*
		{
			auto it = std::ranges::find_if(m_pComponents, [](const auto& pComp)
										   {
											   return dynamic_cast<CompT*>(pComp.get()) != nullptr;
										   });
			return (it != m_pComponents.end()) ? dynamic_cast<CompT*>(it->get()) : nullptr;
		}

		template<Component CompT>
		[[nodiscard]] auto TryGetComponent() const -> std::optional<CompT*>
		{
			if (auto* comp = GetComponent<CompT>())
				return comp;
			return std::nullopt;
		}

	private:
		GameObject* m_pParent{};
		std::vector<GameObject*> m_pChildren{};
		std::vector<std::unique_ptr<BaseComponent>> m_pComponents{};

		std::string m_Name{};
		bool m_MarkedForDelete{};
		bool m_KeepWorldPosition{};
		bool m_PositionDirty{};

		TransformComponent* m_pTransform{ nullptr };

		void RemoveChild(GameObject* pChild);
		void AddChild(GameObject* pChild);
	};
}
