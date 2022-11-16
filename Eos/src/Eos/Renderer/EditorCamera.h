#pragma once

#include "Eos/Renderer/Camera.h"
#include "Eos/Core/Timestep.h"
#include "Eos/Events/Event.h"
#include "Eos/Events/MouseEvent.h"

namespace Eos {

	enum class CameraState
	{
		Default = -1, Pan, Rotate, Zoom
	};

	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);
		
		float GetDistance() const { return m_Distance; }
		void SetDistance(float distance) { m_Distance = distance; }

		void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return m_Position; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }

		void SetViewportHovered(bool viewportHovered) { m_ViewportHovered = viewportHovered; }
		void SetRotationLocked(bool rotationLocked) { m_RotationLocked = rotationLocked; }

		bool IsViewingToolActive() const { return m_ViewingToolActive; }
		CameraState GetCameraState() const { return m_CameraState; }
	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;
		
		float PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	private:
		float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

		glm::mat4 m_ViewMatrix{ 0.0f };
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

		glm::vec2 m_InitialMousePosition = { 0.0, 0.0f };

		float m_Distance = 10.0f;
		float m_Pitch = 0.0f, m_Yaw = 0.0f;

		float m_ViewportWidth = 1280, m_ViewportHeight = 720;

		bool m_ViewingToolActive = false;
		bool m_ViewportHovered = false;
		bool m_RotationLocked = false;

		CameraState m_CameraState = CameraState::Default;
	};

}
