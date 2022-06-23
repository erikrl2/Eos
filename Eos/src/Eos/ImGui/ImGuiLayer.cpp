#include "eospch.h"
#include "Eos/ImGui/ImGuiLayer.h"

#define IMGUI_IMPL_API
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "Eos/Core/Application.h"

#include <ImGuizmo.h>

// temporary
#include <GLFW/glfw3.h>

namespace Eos {

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	void ImGuiLayer::OnAttach()
	{
		EOS_PROFILE_FUNCTION();

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable | ImGuiConfigFlags_NavNoCaptureKeyboard;

		io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Bold.ttf", Window::s_HighDPIScaleFactor * 17.0f);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Regular.ttf", Window::s_HighDPIScaleFactor * 17.0f);

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		style.ScaleAllSizes(Window::s_HighDPIScaleFactor);

		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;

		style.PopupRounding = 5.0f;
		style.FrameRounding = 4.0f;

		SetDarkThemeColors();

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		EOS_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		if (m_BlockEvents)
		{
			ImGuiIO io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::Begin()
	{
		EOS_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::End()
	{
		EOS_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::SetDarkThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.14f, 0.14f, 0.14f, 1.0f };

		// Header
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };

		// Button
		colors[ImGuiCol_Button] = ImVec4{ 0.23f, 0.23f, 0.23f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.33f, 0.33f, 0.33f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.18f, 0.18f, 0.18f, 1.0f };

		// FrameBg
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.06f, 0.06f, 0.06f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.07f, 0.07f, 0.07f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.07f, 0.07f, 0.07f, 1.0f };

		// Tab
		colors[ImGuiCol_Tab] = ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.16f, 0.16f, 0.16f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.09f, 0.09f, 0.09f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.09f, 0.09f, 0.09f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.09f, 0.09f, 0.09f, 1.0f };

		// Docking
		colors[ImGuiCol_DockingPreview] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

		// CheckMark
		colors[ImGuiCol_CheckMark] = ImVec4{ 0.9f, 0.9f, 0.9f, 1.0f };

		// Separator
		colors[ImGuiCol_Separator] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
		colors[ImGuiCol_SeparatorHovered] = ImVec4{ 0.4f, 0.4f, 0.4f, 1.0f };
		colors[ImGuiCol_SeparatorActive] = ImVec4{ 0.4f, 0.4f, 0.4f, 1.0f };

		// Border
		colors[ImGuiCol_Border] = ImVec4{ 0.6f, 0.6f, 0.6f, 1.0f };

		// ResizeGrip
		colors[ImGuiCol_ResizeGrip] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
		colors[ImGuiCol_ResizeGripHovered] = ImVec4{ 0.7f, 0.7f, 0.7f, 1.0f };
		colors[ImGuiCol_ResizeGripActive] = ImVec4{ 0.9f, 0.9f, 0.9f, 1.0f };

		// SliderGrab
		colors[ImGuiCol_SliderGrab] = ImVec4{ 0.5f, 0.5f, 0.5f, 1.0f };
		colors[ImGuiCol_SliderGrabActive] = ImVec4{ 0.6f, 0.6f, 0.6f, 1.0f };

		// Text
		colors[ImGuiCol_Text] = ImVec4{ 0.9f, 0.9f, 0.9f, 1.0f };
		colors[ImGuiCol_TextDisabled] = ImVec4{ 0.4f, 0.4f, 0.4f, 1.0f };
		colors[ImGuiCol_TextSelectedBg] = ImVec4{ 0.07f, 0.3f, 0.7f, 1.0f };

		// MenuBarBg
		colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.09f, 0.09f, 0.09f, 1.0f };

		// PopupBg
		colors[ImGuiCol_PopupBg] = ImVec4{ 0.06f, 0.06f, 0.06f, 1.0f };
	}

}
