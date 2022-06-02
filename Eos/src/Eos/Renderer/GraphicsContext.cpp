#include "eospch.h"
#include "Eos/Renderer/GraphicsContext.h"

#include "Eos/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Eos {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    EOS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		EOS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
