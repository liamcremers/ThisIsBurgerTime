#pragma once
#include "BaseComponent.h"
#include <string>
#include <memory>

#include "Font.h"
#include "Texture2D.h"
#include "TransformComponent.h"

namespace dae
{
    class TextComponent final : public BaseComponent
    {
    public:
        TextComponent(GameObject& parent, std::string text, Font& font);

        void Update() override;
        void Render() const override;

        void SetText(const std::string& text);
        void SetPosition(const float x, const float y);

        TextComponent(const TextComponent& other) = delete;
        TextComponent(TextComponent&& other) = delete;
        TextComponent& operator=(const TextComponent& other) = delete;
        TextComponent& operator=(TextComponent&& other) = delete;

    private:
        bool m_needsUpdate;
        std::string m_text;
        Font& m_font;
        std::unique_ptr<Texture2D> m_textTexture;
        TransformComponent* m_pTransform{ nullptr };
    };
}
