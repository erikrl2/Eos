#pragma once

#include <Eos.h>

class Sandbox2D : public Eos::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Eos::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Eos::Event& e) override;
private:
};
