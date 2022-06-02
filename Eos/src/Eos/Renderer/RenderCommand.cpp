#include "eospch.h"
#include "Eos/Renderer/RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Eos {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}