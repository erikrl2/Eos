#pragma once

#include <glm/glm.hpp>

#include "Eos/Core/KeyCodes.h"
#include "Eos/Core/MouseCodes.h"

namespace Eos {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};

}
