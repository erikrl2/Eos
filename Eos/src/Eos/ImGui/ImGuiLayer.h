#pragma once

#include "Eos/Core/Layer.h"

#include "Eos/Events/ApplicationEvent.h"
#include "Eos/Events/KeyEvent.h"
#include "Eos/Events/MouseEvent.h"

namespace Eos {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }

		void SetDarkThemeColors();
	private:
		bool m_BlockEvents = true;
	};

}
