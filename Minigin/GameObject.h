#pragma once
#include "BaseComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "ComponentsHeader.h"

#include <string>
#include <glm.hpp>
#include <string>
#include <memory>
#include <vector>
#include <optional>
#include <algorithm>
namespace dae
{
	template<typename CompT>
	concept Component = std::derived_from<CompT, BaseComponent>;

	class Texture2D;

	class GameObject /*final*/
	{
	public:
		GameObject();
		//GameObject() = delete;
		GameObject(const std::string& Name);

		virtual ~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		// Core functions
		virtual void Update();
		virtual void FixedUpdate();
		virtual void LateUpdate();
		virtual void Render() const;

		// Parent & Children
		void SetParent(GameObject* pParent);

		[[nodiscard]] GameObject* GetParent() const;
		[[nodiscard]] size_t GetChildCount() const;
		[[nodiscard]] GameObject* GetChildAt(size_t idx) const;

		// Object state management
		void MarkForDelete();

		[[nodiscard]] bool IsMarkedForDelete() const;
		[[nodiscard]] bool HasNoComponents() const;

		// Position management
		void SetPosition(const glm::vec2& pos) const;
		void SetPosition(const float x,const float y) const;

		[[nodiscard]] const glm::vec2& GetPosition() const;
		[[nodiscard]] const std::string& GetName() const;

		template<Component CompT, typename... Args>
		CompT* AddComponent(const Args&... args)
		{
			if (HasComponent<CompT>())
			{
				RemoveComponent<CompT>();
			}
			auto component = std::make_unique<CompT>(this, args...);
			CompT* componentPtr = component.get();
			m_pComponents.emplace_back(std::move(component));
			return componentPtr;
		}

		template<Component CompT>
		void RemoveComponent()
		{
			auto pcomp{ GetComponent<CompT>() };
			auto it = std::find_if(m_pComponents.begin(), m_pComponents.end(), [&](const auto& pComp)
								   {
									   return pComp.get() == pcomp;
								   });
			if (it != m_pComponents.end())
			{
				m_pComponents.erase(it);
			}
		}

		template <Component CompT>
		constexpr [[nodiscard]] bool HasComponent() const
		{
			return std::any_of(m_pComponents.begin(), m_pComponents.end(), [](const auto& pComp)
							   {
								   return dynamic_cast<CompT*>(pComp.get()) != nullptr;
							   });
		}

		template <Component CompT>
		constexpr CompT* GetComponent() const
		{
			for (const auto& pComp : m_pComponents)
			{
				if (auto component = dynamic_cast<CompT*>(pComp.get()))
				{
					return component;
				}
			}
			return nullptr;
		}

		template <Component CompT>
		std::optional<CompT*> TryGetComponent() const
		{
			for (const auto& pComp : m_pComponents)
			{
				if (auto component = dynamic_cast<CompT*>(pComp.get()))
				{
					return component;
				}
			}
			return std::nullopt;
		}
	private:
		std::shared_ptr<GameObject> m_pParent{};
		std::vector<std::unique_ptr<GameObject>> m_pChildren{};
		std::vector<std::unique_ptr<BaseComponent>> m_pComponents{};

		const std::string m_Name{};
		bool m_MarkedForDelete{};
	};
}
