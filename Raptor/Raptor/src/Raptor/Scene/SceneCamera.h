#pragma once

#include "Raptor/Renderer/Camera.h"

namespace Raptor {
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { perspective = 0, orthographic = 1};
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float verticalFov, float nearClip, float farClip);

		void SetviewportSize(uint32_t width, uint32_t heigh);

		float	GetPerspectiveVerticalFOV() const { return m_PerspectiveFov; }
		void	SetPerspectiveVerticalFOV(float verticalFov) { m_PerspectiveFov = verticalFov; ReCalculateProjection(); }
		float	GetPerspectiveNearClip() const { return m_PerspectiveNear; }
		float	GetPerspectiveFarClip() const { return m_PerspectiveFar; }
		void	SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; ReCalculateProjection();}
		void	SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; ReCalculateProjection();}

		float	GetOrthographicsize() const { return m_OrthographicSize; }
		void	SetOrthographicSize(float size) { m_OrthographicSize = size; ReCalculateProjection(); }
		float	GetOrthographicNearClip() const { return m_OrthographicNear; }
		float	GetOrthographicFarClip() const { return m_OrthographicFar; }
		void	SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; ReCalculateProjection(); }
		void	SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; ReCalculateProjection(); }


		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; ReCalculateProjection();}
	private:
		void ReCalculateProjection();
	private:
		ProjectionType m_ProjectionType = ProjectionType::orthographic;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		float m_PerspectiveFov = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

		float m_AscpectRatio = 0.0f;
	};
}
