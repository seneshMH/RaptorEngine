#include "rtpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Raptor {

	SceneCamera::SceneCamera()
	{
		ReCalculateProjection();
	}
	
	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::orthographic;

		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;

		ReCalculateProjection();
	}

	void SceneCamera::SetPerspective(float verticalFov, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::perspective;
		m_PerspectiveFov = verticalFov;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;

		ReCalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t heigh)
	{
		m_AscpectRatio = (float)width / (float)heigh;
		
		ReCalculateProjection();
	}

	void SceneCamera::ReCalculateProjection()
	{
		if (m_ProjectionType == ProjectionType::perspective)
		{
			m_Projection = glm::perspective(m_PerspectiveFov, m_AscpectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}
		else
		{
			float orthoLeft = -m_OrthographicSize * m_AscpectRatio * 0.5f;
			float orthoRight = m_OrthographicSize * m_AscpectRatio * 0.5f;
			float orthoBottom = -m_OrthographicSize * 0.5f;
			float orthoTop = m_OrthographicSize * 0.5f;

			m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
		}

	}

}