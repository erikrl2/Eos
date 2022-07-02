#include "Sandbox2D.h"

#include "Eos/Scene/SceneSerializer.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <box2d/b2_body.h>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D")
{
}

class PlayerScript;

void Sandbox2D::OnAttach()
{
	std::string_view scenePath = "assets/scenes/Test.eos";
	if (!std::filesystem::exists(scenePath))
	{
		EOS_ERROR("Scene file {0} not found!", scenePath);
	}

	m_Scene = Eos::CreateRef<Eos::Scene>();
	Eos::SceneSerializer serializer(m_Scene);
	serializer.Deserialize(scenePath);

	Eos::Application::Get().GetWindow().MaximizeWindow();

	for (auto&& [e, tc] : m_Scene->GetAllEntitiesWith<Eos::TagComponent>().each())
	{
		if (tc.Tag == "Player")
		{
			Eos::Entity player = { e, *m_Scene };
			player.AddComponent<Eos::NativeScriptComponent>();
			player.GetComponent<Eos::NativeScriptComponent>().Bind<PlayerScript>();
			for (int i = 0; i < 50; i++)
				m_Scene->DuplicateEntity(player);
		}
	}

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


class PlayerScript : public Eos::NativeScript
{
public:
	PlayerScript(Eos::Entity entity)
		: Eos::NativeScript(entity) {}

	virtual void OnUpdate(Eos::Timestep ts) override
	{
		b2Body* body = (b2Body*)GetComponent<Eos::Rigidbody2DComponent>().RuntimeBody;

		if (Eos::Input::IsKeyPressed(Eos::Key::A))
			body->ApplyLinearImpulseToCenter(b2Vec2(-0.5f, 0), true);
		if (Eos::Input::IsKeyPressed(Eos::Key::D))
			body->ApplyLinearImpulseToCenter(b2Vec2(0.5f, 0), true);
		if (Eos::Input::IsKeyPressed(Eos::Key::W))
			body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, 0.5f), true);
		if (Eos::Input::IsKeyPressed(Eos::Key::S))
			body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -0.5f), true);
	}
};

