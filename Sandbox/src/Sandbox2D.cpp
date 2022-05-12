#include "Sandbox2D.h"

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

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(16.0f / 9.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION();

	m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
	m_SpriteSheet = Hazel::Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");

	m_TextureGrass = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1, 11 }, { 128, 128 });
	m_TextureTree = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 4, 1 }, { 128, 128 }, { 1, 2 });

	m_MapWidth = s_MapWidth;
	m_MapHeight = strlen(s_MapTiles) / s_MapWidth;

	s_TextureMap['W'] = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, {11, 11}, {128, 128});
	s_TextureMap['D'] = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, {6, 11}, {128, 128});

	m_CameraController.SetZoomLevel(5.0f);
}

void Sandbox2D::OnDetach()
{
	HZ_PROFILE_FUNCTION();

}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
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

	#if 0
	{
		static float rotation = 0.0f;
		rotation += 360.0f * ts;
		if (rotation > 360.0f)
			rotation = 0.0f;
		HZ_PROFILE_SCOPE("Renderer Draw");
		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Hazel::Renderer2D::DrawRotatedQuad({ -0.5f, 0.5f }, { 0.8f, 0.8f }, glm::radians(rotation), { 0.8f, 0.2f, 0.3f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ -0.5f, 0.5f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ 1.0f, 0.0f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 10.0f);
		Hazel::Renderer2D::DrawRotatedQuad(glm::vec3(0.0f), { 1.0f, 1.0f }, glm::radians(rotation), m_CheckerboardTexture, 5.0f, { 1.0f, 0.8f, 0.9f, 1.0f });
		for (float y = -10.0f; y <= 10.0f; y += 0.5f)
		{
			for (float x = -10.0f; x <= 10.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 10.0f) / 20.0f, 0.4f, (y + 10.0f) / 20.0f, 0.8f };
				Hazel::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}
		Hazel::Renderer2D::EndScene();
	}
	#endif

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

	#if 0
	Hazel::Renderer2D::DrawQuad({ -1.0f, 0.5f }, { 1.0f, 2.0f }, m_TextureTree);
	Hazel::Renderer2D::DrawQuad(glm::vec2(0.0f), glm::vec2(1.0f), m_TextureStairs);
	Hazel::Renderer2D::DrawQuad({ 1.0f, 0.0f }, glm::vec2(1.0f), m_TextureGrass);
	#endif

	#if 0
	if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_0))
	{
		const auto& window = Hazel::Application::Get().GetWindow();
		const auto& bounds = m_CameraController.GetBounds();
		const float x = (Hazel::Input::GetMouseX() / window.GetWidth() * bounds.GetWidth()) - bounds.Right;
		const float y = -((Hazel::Input::GetMouseY() / window.GetHeight() * bounds.GetHeight()) - bounds.Top);
		const float size = m_CameraController.GetZoomLevel() / 10.0f;
		Hazel::Renderer2D::DrawQuad({ x, y, 0.5f }, glm::vec2(size), glm::vec4(1.0f));
	}
	#endif
	Hazel::Renderer2D::EndScene();

}

void Sandbox2D::OnImGuiRender()
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

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}
