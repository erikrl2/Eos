#include "ExampleLayer.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static const uint32_t s_MapWidth = 25;
static const char* s_MapTiles =
"WWWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWDDWWWWWWWWWWWWWW"
"WWWWWWWDDDDDDDWWWWWWWWWWW"
"WWWWWDDDDDDDDDDDWWWWWWWWW"
"WWWWDDDDDDDDDDDDDDDWWWWWW"
"WWWDDDWWWDDDDDDDDDDDWWWWW"
"WWWDDDWWWDDDDDDDDDDDDWWWW"
"WWDDDDDDDDDDDDDXXXDDDDWWW"
"WWDDDDDDDDDDDDDDXXDDDWWWW"
"WWWDDDDDDDDDDDDDDDDDWWWWW"
"WWWWDDDDDDDDDDDDDDDWWWWWW"
"WWWWWWDDDDDDDDDDDWWWWWWWW"
"WWWWWWWDDDDDDDDWWWWWWWWWW"
"WWWWWWWWWDDDWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWWW"
;

ExampleLayer::ExampleLayer()
	: Layer("Example"), m_CameraController(16.0f / 9.0f, true), m_MapWidth(0), m_MapHeight(0)
{
}

void ExampleLayer::OnAttach()
{
	HZ_PROFILE_FUNCTION();

	m_SpriteSheet = Hazel::Texture2D::Create("assets/textures/RPGpack_sheet_2X.png");

	m_TextureGrass = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1, 11 }, { 128, 128 });
	m_TextureTree = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 4, 1 }, { 128, 128 }, { 1, 2 });

	m_MapWidth = s_MapWidth;
	m_MapHeight = strlen(s_MapTiles) / s_MapWidth;

	s_TextureMap['W'] = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11, 11 }, { 128, 128 });
	s_TextureMap['D'] = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 11 }, { 128, 128 });

	m_CameraController.SetZoomLevel(5.0f);
}

void ExampleLayer::OnDetach()
{
	HZ_PROFILE_FUNCTION();

}

void ExampleLayer::OnUpdate(Hazel::Timestep ts)
{
	HZ_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	Hazel::Renderer2D::ResetStats();
	{
		HZ_PROFILE_SCOPE("Renderer Prep");
		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Hazel::RenderCommand::Clear();
	}

	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());

	for (uint32_t y = 0; y < m_MapHeight; y++)
	{
		for (uint32_t x = 0; x < m_MapWidth; x++)
		{
			char tileType = s_MapTiles[x + y * m_MapWidth];
			Hazel::Ref<Hazel::SubTexture2D> texture;
			if (s_TextureMap.find(tileType) != s_TextureMap.end())
				texture = s_TextureMap[tileType];
			else
				texture = m_TextureGrass;
			Hazel::Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f + 0.5f, m_MapHeight - y - m_MapHeight / 2.0f - 0.5f }, glm::vec2(1.0f), texture);
		}
	}

	Hazel::Renderer2D::DrawQuad({ -1.0f, 0.5f }, { 1.0f, 2.0f }, m_TextureTree);
	Hazel::Renderer2D::DrawQuad({ 1.0f, 0.0f }, glm::vec2(1.0f), m_TextureGrass);

	if (Hazel::Input::IsMouseButtonPressed(Hazel::Mouse::Button0))
	{
		static float rotation = 0.0f;
		rotation += 360.0f * ts;
		if (rotation > 360.0f)
			rotation = 0.0f;
		const auto& window = Hazel::Application::Get().GetWindow();
		const auto& bounds = m_CameraController.GetBounds();
		const float x = (Hazel::Input::GetMouseX() / window.GetWidth() * bounds.GetWidth()) - bounds.Right;
		const float y = -((Hazel::Input::GetMouseY() / window.GetHeight() * bounds.GetHeight()) - bounds.Top);
		const float size = m_CameraController.GetZoomLevel() / 10.0f;
		Hazel::Renderer2D::DrawRotatedQuad({ x, y, 0.5f }, glm::vec2(size), glm::radians(rotation), glm::vec4(1.0f));
	}
	Hazel::Renderer2D::EndScene();

}

void ExampleLayer::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	auto stats = Hazel::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::End();
}

void ExampleLayer::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}
