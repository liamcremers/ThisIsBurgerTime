#pragma once
#include "BaseComponent.h"
#include "GameObject.h"
#include <string>
#include <memory>

namespace dae
{
	class Font;
	class Texture2D;
	class TextComponent final : public BaseComponent
	{
	public:
		TextComponent(GameObject* parent, const std::string& text, std::shared_ptr<Font> font);

		void Update() override;
		void Render() const override;

		void SetText(const std::string& text);

		void SetPosition(const float x, const float y);

		virtual ~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;
	private:
		bool m_needsUpdate;
		std::string m_text;
		std::shared_ptr<Font> m_font;
		std::shared_ptr<Texture2D> m_textTexture;

		void FixedUpdate() override {};
		void LateUpdate() override {};
	};
}
