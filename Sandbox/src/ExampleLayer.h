#pragma once

#include <Hazel.h>

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Hazel::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Hazel::Event& e) override;
private:
	Hazel::OrthographicCameraController m_CameraController;

	Hazel::Ref<Hazel::Texture2D> m_CheckerboardTexture;
	Hazel::Ref<Hazel::Texture2D> m_SpriteSheet;
	Hazel::Ref<Hazel::SubTexture2D> m_TextureGrass, m_TextureTree;

	uint32_t m_MapWidth, m_MapHeight;
	std::unordered_map<char, Hazel::Ref<Hazel::SubTexture2D>> s_TextureMap;
};
