#include "Sandbox2D.h"

#include "Eos/Scene/SceneSerializer.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D")
{
}

static class PlayerScript;

void Sandbox2D::OnAttach()
{
	m_Scene = Eos::CreateRef<Eos::Scene>();
	Eos::SceneSerializer serializer(m_Scene);
	if (!serializer.Deserialize("assets/scenes/Test.eos"))
		EOS_ASSERT(false);

	Eos::Window& window = Eos::Application::Get().GetWindow();
	m_Scene->OnViewportResize(window.GetWidth(), window.GetHeight());

	m_Player = m_Scene->CreateEntity("Player")
		.AddComponent<Eos::SpriteRendererComponent>(glm::vec4(0.2f, 0.8f, 0.3f, 1.0f))
		.AddComponent<Eos::NativeScriptComponent>();
	m_Player.GetComponent<Eos::NativeScriptComponent>().Bind<PlayerScript>();

	m_Scene->OnRuntimeStart();
}

void Sandbox2D::OnDetach()
{
	m_Scene->OnRuntimeStop();
}

void Sandbox2D::OnUpdate(Eos::Timestep ts)
{
	Eos::RenderCommand::SetClearColor({ 0.3f, 0.1f, 0.1f, 1.0f });
	Eos::RenderCommand::Clear();

	m_Scene->OnUpdateRuntime(ts);
}

void Sandbox2D::OnEvent(Eos::Event& e)
{
	Eos::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Eos::WindowResizeEvent>(EOS_BIND_EVENT_FN(OnWindowResize));
}

bool Sandbox2D::OnWindowResize(Eos::WindowResizeEvent& e)
{
	m_Scene->OnViewportResize(e.GetWidth(), e.GetHeight());
	return false;
}


static class PlayerScript : public Eos::NativeScript
{
public:
	PlayerScript(Eos::Entity entity)
		: Eos::NativeScript(entity), m_Player(entity)
	{
	}

	~PlayerScript()
	{
	}

	virtual void OnUpdate(Eos::Timestep ts) override
	{
		auto& transform = m_Player.GetComponent<Eos::TransformComponent>();
		if (Eos::Input::IsKeyPressed(Eos::Key::A))
			transform.Translation.x -= 10.0f * ts;
		if (Eos::Input::IsKeyPressed(Eos::Key::D))
			transform.Translation.x += 10.0f * ts;
		if (Eos::Input::IsKeyPressed(Eos::Key::W))
			transform.Translation.y += 10.0f * ts;
		if (Eos::Input::IsKeyPressed(Eos::Key::S))
			transform.Translation.y -= 10.0f * ts;
	}
private:
	Eos::Entity m_Player;
};

