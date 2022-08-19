#include "Sandbox2D.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D")
{
}

void Sandbox2D::OnAttach()
{
	m_Scene = Eos::CreateRef<Eos::Scene>();
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

void Sandbox2D::OnImGuiRender()
{
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

