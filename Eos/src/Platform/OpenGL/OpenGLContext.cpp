#include "eospch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include "glad/glad.h"

namespace Eos {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		EOS_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		EOS_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		EOS_CORE_ASSERT(status, "Failed to initialize Glad!");

		EOS_CORE_INFO("OpenGL Info:");
		EOS_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		EOS_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		EOS_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

		EOS_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Eos requires at least OpenGL version 4.5!");
	}

	void OpenGLContext::SwapBuffers()
	{
		EOS_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}


}
